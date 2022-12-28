#include <iostream>
#include <vector>
#include <aws/core/Aws.h>
#include "src/net/Server.h"
#include "src/storage/StorageService.h"
#include "src/db/Table.h"
// #include <nlohmann/json.hpp>
// using json = nlohmann::json;
#include "src/sql/lexer/Lexer.h"
#include "src/sql/code/Repository.h"
#include "src/sql/parser/Parser.h"
#include "src/sql/ast/AST.h"
#include "src/sql/ast/Evaluator.h"

int main()
{
    /*Server server(4000, "0.0.0.0");
    server.bind();

    while (true) {
        int client_socket = server.accept();
        std::string message = server.receive(client_socket);
        std::cout << "Received: " << message << std::endl;
        json j = json::parse(message);
        std::cout << j["name"] << ": " << j["age"] << std::endl;
        server.send(client_socket, "Hello from server");
        server.close(client_socket);
    }
    return 0;*/

    /*Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;
    Aws::InitAPI(options);
    {

    std::string database = "cpp_test";

    StorageService storageService("serverless-db-9umfiaj");

    std::vector<Column> user_columns;

    user_columns.emplace_back(Column{"id", DataType::VARCHAR, 36, true});
    user_columns.emplace_back(Column{"name", DataType::VARCHAR, 255, false});
    user_columns.emplace_back(Column{"age", DataType::INTEGER, 4, false});

    Table table("user", database, user_columns, storageService);*/

    /*std::vector<std::shared_ptr<DataEntryBase>> user1;
    user1.emplace_back(std::make_shared<DataEntry<std::string>>("1", DataType::VARCHAR));
    user1.emplace_back(std::make_shared<DataEntry<std::string>>("John", DataType::VARCHAR));
    user1.emplace_back(std::make_shared<DataEntry<int32_t>>(20, DataType::INTEGER));

    std::vector<std::shared_ptr<DataEntryBase>> user2;
    user2.emplace_back(std::make_shared<DataEntry<std::string>>("2", DataType::VARCHAR));
    user2.emplace_back(std::make_shared<DataEntry<std::string>>("Jane", DataType::VARCHAR));
    user2.emplace_back(std::make_shared<DataEntry<int32_t>>(21, DataType::INTEGER));

    table.insertItem(user1);
    table.insertItem(user2);*/

    /*table.getItems();

    } // The SDK must be shutdown before the application terminates.
    Aws::ShutdownAPI(options);*/

    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;
    Aws::InitAPI(options);
    {

        Repository repo("SELECT vorname AS name, age FROM user LEFT JOIN posts ON post = user;");
        //Repository repo("UPDATE user SET age = 25, name = 'Maxi' WHERE id = '1' AND 5 < 6;");
        sql::lexer::Lexer lexer(repo);

        /*while (lexer.has_next_token()) {
            Token token = lexer.next();
            std::cout << token << std::endl;
        }*/

        sql::parser::Parser parser(lexer, repo);
        parser.parse();

        parser.print(std::cout);

        sql::ast::AST ast(parser, repo);
        ast.analyze();

        ast.print(std::cout);

        /*std::string database = "cpp_test";
        StorageService storageService("serverless-db-9umfiaj");
        Database db(database, storageService);

        sql::ast::Evaluator evalutor(ast, db);

        EvaluationTable result = evalutor.evaluate();

        result.print(std::cout);*/
    } // The SDK must be shutdown before the application terminates.
    Aws::ShutdownAPI(options);

    return 0;
}
