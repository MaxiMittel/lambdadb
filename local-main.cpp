#include <iostream>
#include <vector>
#include <aws/core/Aws.h>
#include "src/net/Server.h"
#include "src/storage/StorageService.h"
#include "src/db/Table.h"
#include "src/sql/lexer/Lexer.h"
#include "src/sql/code/Repository.h"
#include "src/sql/parser/Parser.h"
#include "src/sql/ast/AST.h"
#include "src/sql/ast/Evaluator.h"

int main()
{
    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Info;
    Aws::InitAPI(options);
    {

        std::string database = "user_products";
        std::string query = "SELECT * FROM user;";

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

        result.print(std::cout);

    } // The SDK must be shutdown before the application terminates.
    Aws::ShutdownAPI(options);

    return 0;
}
