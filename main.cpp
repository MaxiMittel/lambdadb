#include <iostream>
#include <vector>
#include <aws/core/Aws.h>
#include "src/net/Server.h"
#include "src/storage/StorageService.h"
#include "src/db/Table.h"
#include <nlohmann/json.hpp>
#include "src/sql/lexer/Lexer.h"
#include "src/sql/code/Repository.h"
#include "src/sql/parser/Parser.h"
#include "src/sql/ast/AST.h"
#include "src/sql/ast/Evaluator.h"

using json = nlohmann::json;

enum MessageAction {
    QUERY,
};

int main()
{
    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;
    Aws::InitAPI(options);
    {
        Server server(4000, "0.0.0.0");
        server.bind();

        while (true) {
            int client_socket = server.accept();
            std::string message = server.receive(client_socket);
            json j = json::parse(message);

            MessageAction action = j["action"];

            switch (action) {
                case QUERY: {
                    std::string database = j["database"];
                    std::string query = j["query"];

                    Repository repo(query);
                    sql::lexer::Lexer lexer(repo);


                    sql::parser::Parser parser(lexer, repo);
                    parser.parse();

                    sql::ast::AST ast(parser, repo);
                    ast.analyze();

                    StorageService storageService("serverless-db-9umfiaj");
                    db::Database db(database, storageService);

                    sql::ast::Evaluator evalutor(ast, db);

                    db::EvaluationTable result = evalutor.evaluate();

                    server.send(client_socket, result.toJSON() + '\n');
                    break;
                }
                default:
                    break;
            }
            server.close(client_socket);
        }
        return 0;

    } // The SDK must be shutdown before the application terminates.
    Aws::ShutdownAPI(options);
}
