#include <iostream>
#include <vector>
#include <aws/core/Aws.h>
#include <aws/lambda-runtime/runtime.h>
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
using namespace aws::lambda_runtime;

enum MessageAction
{
    QUERY,
};

invocation_response main_handler(invocation_request const &request)
{
    std::string response = "";

    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;
    Aws::InitAPI(options);
    {

        json j = json::parse(request.payload);

        MessageAction action = j["action"];

        switch (action)
        {
        case QUERY:
        {
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

            response = result.toJSON() + '\n';
            break;
        }
        default:
            break;
        }
    } // The SDK must be shutdown before the application terminates.
    Aws::ShutdownAPI(options);

    return invocation_response::success(response, "application/json");
}

int main()
{
    run_handler(main_handler);
    return 0;
}