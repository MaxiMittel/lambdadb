//
// Created by Maximilian Mittelhammer on 05.12.22.
//

#include "Parser.h"
#include "PrintVisitor.h"
#include <cassert>

using namespace sql::parser;
using namespace sql::lexer;

Parser::Parser(Lexer& lexer, Repository& repository) : lexer(lexer), repository(repository) {}

bool Parser::parse() {
    root = parse_stmt();
    return success;
}

std::shared_ptr<Node> Parser::getRoot() const {
    return root;
}

void Parser::print(std::ostream& out) {
    PrintVisitor visitor(out);

    out << "digraph PARSER {" << std::endl;
    this->root->accept(visitor);
    out << "}" << std::endl;
}

void Parser::assertTokenType(TokenType expected, TokenType actual, Position position) {
    if (expected != actual) {
        repository.logError(position, "Expected token type " + std::to_string(expected) + " but got " + std::to_string(actual));
        success = false;
        assert(false);
    }
}

std::shared_ptr<Node> Parser::parse_stmt() {
    std::shared_ptr<NodeStmt> stmt = std::make_shared<NodeStmt>(Position{0,0});

    auto token = lexer.peek_next();

    switch (token.type) {
        case TokenType::SELECT:
            stmt->select_stmt = parse_select_stmt();
            break;
        case TokenType::INSERT:
            stmt->insert_stmt = parse_insert_stmt();
            break;
        case TokenType::DELETE:
            stmt->delete_stmt = parse_delete_stmt();
            break;
        case TokenType::UPDATE:
            stmt->update_stmt = parse_update_stmt();
            break;
        case TokenType::CREATE:
            stmt->create_stmt = parse_create_stmt();
            break;
        case TokenType::DROP:
            stmt->drop_stmt = parse_drop_stmt();
            break;
        case TokenType::ALTER:
            stmt->alter_stmt = parse_alter_stmt();
            break;
        default:
            repository.logError(token.position, "Unexpected token: " + std::string(token.value));
            success = false;
            return nullptr;
    }

    return stmt;
}

std::shared_ptr<Node> Parser::parse_select_stmt() {
    auto token = lexer.next();
    std::shared_ptr<NodeSelectStmt> select_stmt = std::make_shared<NodeSelectStmt>(token.position);

    select_stmt->_select = std::make_shared<NodeSelect>(token.position);
    select_stmt->select_list = parse_select_list();

    token = lexer.next();
    assertTokenType(TokenType::FROM, token.type, token.position);
    select_stmt->_from = std::make_shared<NodeFrom>(token.position);
    
    select_stmt->table_list = parse_table_list();

    while (lexer.has_next_token()) {
        token = lexer.peek_next();
        switch (token.type) {
            case TokenType::LEFT:
            case TokenType::RIGHT:
            case TokenType::INNER:
            case TokenType::FULL:
                select_stmt->join_clause = parse_join_clause();
                break;
            case TokenType::WHERE:
                select_stmt->where_clause = parse_where_clause();
                break;
            case TokenType::ORDER:
                select_stmt->order_by_clause = parse_order_clause();
                break;
            case TokenType::LIMIT:
                select_stmt->limit_clause = parse_limit_clause();
                break;
            default:
                break;
        }
    }

    return select_stmt;
}

std::shared_ptr<Node> Parser::parse_select_list() {
    auto token = lexer.peek_next();
    std::shared_ptr<NodeSelectList> select_list = std::make_shared<NodeSelectList>(token.position);

    if (token.type == TokenType::ASTERISK) {
        lexer.next();
        select_list->asteriks = std::make_shared<NodeAsteriks>(token.position);
        return select_list;
    }

    select_list->select_list_items.emplace_back(parse_select_list_item());

    token = lexer.peek_next();
    while (token.type == TokenType::COMMA) {
        lexer.next();
        select_list->select_list_items.emplace_back(parse_select_list_item());
        token = lexer.peek_next();
    }

    return select_list;
}

std::shared_ptr<Node> Parser::parse_select_list_item() {
    auto token = lexer.next();
    std::shared_ptr<NodeSelectListItem> select_list_item = std::make_shared<NodeSelectListItem>(token.position);

    select_list_item->expr = std::make_shared<NodeIdentifier>(token.position, token.value);

    token = lexer.peek_next();
    if (token.type == TokenType::AS) {
        lexer.next();
        token = lexer.next();
        select_list_item->_as = std::make_shared<NodeAs>(token.position);
        select_list_item->identifier = std::make_shared<NodeIdentifier>(token.position, token.value);
    }

    return select_list_item;
}

std::shared_ptr<Node> Parser::parse_table_list() {
    auto token = lexer.peek_next();
    std::shared_ptr<NodeTableList> table_list = std::make_shared<NodeTableList>(token.position);

    table_list->table_list_items.emplace_back(parse_table_list_item());

    token = lexer.peek_next();
    while (token.type == TokenType::COMMA) {
        lexer.next();
        table_list->table_list_items.emplace_back(parse_table_list_item());
        token = lexer.peek_next();
    }

    return table_list;
}

std::shared_ptr<Node> Parser::parse_table_list_item() {
    auto token = lexer.next();
    std::shared_ptr<NodeTableListItem> table_list_item = std::make_shared<NodeTableListItem>(token.position);

    table_list_item->table_name = std::make_shared<NodeIdentifier>(token.position, token.value);

    token = lexer.peek_next();
    if (token.type == TokenType::IDENTIFIER) {
        lexer.next();
        token = lexer.next();
        table_list_item->as = std::make_shared<NodeIdentifier>(token.position, token.value);
    }

    return table_list_item;
}

std::shared_ptr<Node> Parser::parse_join_clause() {
    auto token = lexer.next();
    std::shared_ptr<NodeJoinClause> join_clause = std::make_shared<NodeJoinClause>(token.position);

    switch (token.type) {
        case TokenType::LEFT:
            join_clause->join_type = std::make_shared<NodeLeft>(token.position);
            break;
        case TokenType::RIGHT:
            join_clause->join_type = std::make_shared<NodeRight>(token.position);
            break;
        case TokenType::INNER:
            join_clause->join_type = std::make_shared<NodeInner>(token.position);
            break;
        case TokenType::FULL:
            join_clause->join_type = std::make_shared<NodeFull>(token.position);
            break;
        default:
            repository.logError(token.position, "Unexpected token: " + std::string(token.value));
            success = false;
            return nullptr;
    }

    token = lexer.next();
    assertTokenType(TokenType::JOIN, token.type, token.position);
    join_clause->_join = std::make_shared<NodeJoin>(token.position);

    join_clause->table_list_item = parse_table_list_item();

    token = lexer.next();
    assertTokenType(TokenType::ON, token.type, token.position);
    join_clause->_on = std::make_shared<NodeOn>(token.position);

    join_clause->left = parse_primary_expr();

    token = lexer.next();
    join_clause->_equal = std::make_shared<NodeEqual>(token.position);

    join_clause->right = parse_primary_expr();

    return join_clause;
}

std::shared_ptr<Node> Parser::parse_where_clause() {
    auto token = lexer.next();
    std::shared_ptr<NodeWhereClause> where_clause = std::make_shared<NodeWhereClause>(token.position);

    assertTokenType(TokenType::WHERE, token.type, token.position);
    where_clause->_where = std::make_shared<NodeWhere>(token.position);

    where_clause->expr = parse_expr();

    return where_clause;
}

std::shared_ptr<Node> Parser::parse_order_clause() {
    auto token = lexer.next();
    std::shared_ptr<NodeOrderClause> order_clause = std::make_shared<NodeOrderClause>(token.position);

    assertTokenType(TokenType::ORDER, token.type, token.position);
    order_clause->_order = std::make_shared<NodeOrder>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::BY, token.type, token.position);
    order_clause->_by = std::make_shared<NodeBy>(token.position);

    order_clause->order_list = parse_order_list();

    return order_clause;
}

std::shared_ptr<Node> Parser::parse_order_list() {
    auto token = lexer.next();
    std::shared_ptr<NodeOrderList> order_list = std::make_shared<NodeOrderList>(token.position);

    order_list->order_list_items.emplace_back(parse_order_list_item());

    token = lexer.peek_next();
    while (token.type == TokenType::COMMA) {
        lexer.next();
        order_list->order_list_items.emplace_back(parse_order_list_item());
        token = lexer.peek_next();
    }

    return order_list;
}

std::shared_ptr<Node> Parser::parse_order_list_item() {
    auto token = lexer.next();
    std::shared_ptr<NodeOrderListItem> order_list_item = std::make_shared<NodeOrderListItem>(token.position);

    order_list_item->expr = parse_expr();

    token = lexer.peek_next();
    if (token.type == TokenType::ASC) {
        lexer.next();
        order_list_item->_asc = std::make_shared<NodeAsc>(token.position);
    }

    if (token.type == TokenType::DESC) {
        lexer.next();
        order_list_item->_desc = std::make_shared<NodeDesc>(token.position);
    }

    return order_list_item;
}

std::shared_ptr<Node> Parser::parse_limit_clause() {
    auto token = lexer.next();
    std::shared_ptr<NodeLimitClause> limit_clause = std::make_shared<NodeLimitClause>(token.position);

    assertTokenType(TokenType::LIMIT, token.type, token.position);
    limit_clause->_limit = std::make_shared<NodeLimit>(token.position);

    token = lexer.next();
    limit_clause->expr = parse_expr();

    return limit_clause;
}

std::shared_ptr<Node> Parser::parse_expr() {
    auto token = lexer.peek_next();
    std::shared_ptr<NodeExpression> expr = std::make_shared<NodeExpression>(token.position);
    
    expr->and_expr = parse_and_expr();

    return expr;
}

std::shared_ptr<Node> Parser::parse_and_expr() {
    auto token = lexer.peek_next();
    std::shared_ptr<NodeAndExpression> and_expr = std::make_shared<NodeAndExpression>(token.position);

    and_expr->or_expr = parse_or_expr();

    token = lexer.peek_next();
    while (token.type == TokenType::AND) {
        lexer.next();
        and_expr->_and = std::make_shared<NodeAnd>(token.position);
        and_expr->and_expr = parse_and_expr();
        token = lexer.peek_next();
    }

    return and_expr;
}

std::shared_ptr<Node> Parser::parse_or_expr() {
    auto token = lexer.peek_next();
    std::shared_ptr<NodeOrExpression> or_expr = std::make_shared<NodeOrExpression>(token.position);

    or_expr->bool_expr = parse_bool_expr();

    token = lexer.peek_next();
    while (token.type == TokenType::OR) {
        lexer.next();
        or_expr->_or = std::make_shared<NodeOr>(token.position);
        or_expr->or_expr = parse_or_expr();
        token = lexer.peek_next();
    }

    return or_expr;
}

std::shared_ptr<Node> Parser::parse_bool_expr() {
    auto token = lexer.peek_next();
    std::shared_ptr<NodeBoolExpression> bool_expr = std::make_shared<NodeBoolExpression>(token.position);

    bool_expr->left = parse_unary_expr();

    token = lexer.next();
    switch (token.type) {
        case TokenType::EQUAL:
            bool_expr->_equal = std::make_shared<NodeEqual>(token.position);
            break;
        case TokenType::NOT_EQUAL:
            bool_expr->_not_equal = std::make_shared<NodeNotEqual>(token.position);
            break;
        case TokenType::LESS:
            bool_expr->_less = std::make_shared<NodeLess>(token.position);
            break;
        case TokenType::LESS_EQUAL:
            bool_expr->_less_equal = std::make_shared<NodeLessEqual>(token.position);
            break;
        case TokenType::GREATER:
            bool_expr->_greater = std::make_shared<NodeGreater>(token.position);
            break;
        case TokenType::GREATER_EQUAL:
            bool_expr->_greater_equal = std::make_shared<NodeGreaterEqual>(token.position);
            break;
        default:
            break;
    }

    bool_expr->right = parse_unary_expr();

    return bool_expr;
}

std::shared_ptr<Node> Parser::parse_unary_expr() {
    auto token = lexer.peek_next();
    std::shared_ptr<NodeUnaryExpression> unary_expr = std::make_shared<NodeUnaryExpression>(token.position);

    switch (token.type) {
        case TokenType::PLUS:
            unary_expr->_plus = std::make_shared<NodePlus>(token.position);
            break;
        case TokenType::MINUS:
            unary_expr->_minus = std::make_shared<NodeMinus>(token.position);
            break;
        case TokenType::NOT:
            unary_expr->_not = std::make_shared<NodeNot>(token.position);
            break;
        default:
            return parse_primary_expr();
    }

    lexer.next();
    unary_expr->primary_expr = parse_primary_expr();

    return unary_expr;
}

std::shared_ptr<Node> Parser::parse_primary_expr() {
    auto token = lexer.peek_next();
    std::shared_ptr<NodePrimaryExpression> primary_expr = std::make_shared<NodePrimaryExpression>(token.position);

    switch (token.type) {
        case TokenType::INTEGER_LITERAL:
            lexer.next();
            primary_expr->integer_literal = std::make_shared<NodeIntegerLiteral>(token.position, token.value);
            break;
        case TokenType::FLOAT_LITERAL:
            lexer.next();
            primary_expr->float_literal = std::make_shared<NodeFloatLiteral>(token.position, token.value);
            break;
        case TokenType::STRING_LITERAL:
            lexer.next();
            primary_expr->string_literal = std::make_shared<NodeStringLiteral>(token.position, token.value);
            break;
        case TokenType::IDENTIFIER:
            lexer.next();
            primary_expr->ident1 = std::make_shared<NodeIdentifier>(token.position, token.value);
            token = lexer.peek_next();
            if (token.type == TokenType::DOT) {
                lexer.next();
                primary_expr->_dot = std::make_shared<NodeDot>(token.position);
                token = lexer.next();
                assertTokenType(TokenType::IDENTIFIER, token.type, token.position);
                primary_expr->ident2 = std::make_shared<NodeIdentifier>(token.position, token.value);
            }
            break;
        case TokenType::BRACE_OPEN:
            lexer.next();
            primary_expr->_lparen = std::make_shared<NodeBraceOpen>(token.position);
            primary_expr->expr = parse_expr();
            token = lexer.next();
            assertTokenType(TokenType::BRACE_CLOSE, token.type, token.position);
            primary_expr->_rparen = std::make_shared<NodeBraceClose>(token.position);
            break;
        default:
            repository.logError(token.position, "Unexpected token: " + std::string(token.value));
            success = false;
            assert(false);
            break;
    }

    return primary_expr;
}

std::shared_ptr<Node> Parser::parse_insert_stmt() {
    auto token = lexer.next();
    std::shared_ptr<NodeInsertStmt> insert_stmt = std::make_shared<NodeInsertStmt>(token.position);

    assertTokenType(TokenType::INSERT, token.type, token.position);
    insert_stmt->_insert = std::make_shared<NodeInsert>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::INTO, token.type, token.position);
    insert_stmt->_into = std::make_shared<NodeInto>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::IDENTIFIER, token.type, token.position);
    insert_stmt->table_name = std::make_shared<NodeIdentifier>(token.position, token.value);

    token = lexer.next();
    assertTokenType(TokenType::BRACE_OPEN, token.type, token.position);
    insert_stmt->brace_open_1 = std::make_shared<NodeBraceOpen>(token.position);

    insert_stmt->column_list = parse_column_list();

    token = lexer.next();
    assertTokenType(TokenType::BRACE_CLOSE, token.type, token.position);
    insert_stmt->brace_close_1 = std::make_shared<NodeBraceClose>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::VALUES, token.type, token.position);
    insert_stmt->_values = std::make_shared<NodeValues>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::BRACE_OPEN, token.type, token.position);
    insert_stmt->brace_open_2 = std::make_shared<NodeBraceOpen>(token.position);

    insert_stmt->values = parse_value_list();

    token = lexer.next();
    assertTokenType(TokenType::BRACE_CLOSE, token.type, token.position);
    insert_stmt->brace_close_2 = std::make_shared<NodeBraceClose>(token.position);

    return insert_stmt;
}

std::shared_ptr<Node> Parser::parse_update_stmt() {
    auto token = lexer.next();
    std::shared_ptr<NodeUpdateStmt> update_stmt = std::make_shared<NodeUpdateStmt>(token.position);

    assertTokenType(TokenType::UPDATE, token.type, token.position);
    update_stmt->_update = std::make_shared<NodeUpdate>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::IDENTIFIER, token.type, token.position);
    update_stmt->table_name = std::make_shared<NodeIdentifier>(token.position, token.value);

    token = lexer.next();
    assertTokenType(TokenType::SET, token.type, token.position);
    update_stmt->_set = std::make_shared<NodeSet>(token.position);

    update_stmt->update_list = parse_assignment_list();

    update_stmt->where_clause = parse_where_clause();

    return update_stmt;
}

std::shared_ptr<Node> Parser::parse_delete_stmt() {
    auto token = lexer.next();
    std::shared_ptr<NodeDeleteStmt> delete_stmt = std::make_shared<NodeDeleteStmt>(token.position);

    assertTokenType(TokenType::DELETE, token.type, token.position);
    delete_stmt->_delete = std::make_shared<NodeDelete>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::FROM, token.type, token.position);
    delete_stmt->_from = std::make_shared<NodeFrom>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::IDENTIFIER, token.type, token.position);
    delete_stmt->table_name = std::make_shared<NodeIdentifier>(token.position, token.value);

    delete_stmt->where_clause = parse_where_clause();

    return delete_stmt;
}

std::shared_ptr<Node> Parser::parse_create_stmt() {
    auto token = lexer.next();
    std::shared_ptr<NodeCreateStmt> create_stmt = std::make_shared<NodeCreateStmt>(token.position);

    assertTokenType(TokenType::CREATE, token.type, token.position);
    create_stmt->_create = std::make_shared<NodeCreate>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::TABLE, token.type, token.position);
    create_stmt->_table = std::make_shared<NodeTable>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::IDENTIFIER, token.type, token.position);
    create_stmt->table_name = std::make_shared<NodeIdentifier>(token.position, token.value);

    token = lexer.next();
    assertTokenType(TokenType::BRACE_OPEN, token.type, token.position);
    create_stmt->_brace_open = std::make_shared<NodeBraceOpen>(token.position);

    create_stmt->column_def = parse_column_def_list();

    token = lexer.next();
    assertTokenType(TokenType::BRACE_CLOSE, token.type, token.position);
    create_stmt->_brace_close = std::make_shared<NodeBraceClose>(token.position);

    return create_stmt;
}

std::shared_ptr<Node> Parser::parse_drop_stmt() {
    auto token = lexer.next();
    std::shared_ptr<NodeDropStmt> drop_stmt = std::make_shared<NodeDropStmt>(token.position);

    assertTokenType(TokenType::DROP, token.type, token.position);
    drop_stmt->_drop = std::make_shared<NodeDrop>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::TABLE, token.type, token.position);
    drop_stmt->_table = std::make_shared<NodeTable>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::IDENTIFIER, token.type, token.position);
    drop_stmt->table_name = std::make_shared<NodeIdentifier>(token.position, token.value);

    return drop_stmt;
}

std::shared_ptr<Node> Parser::parse_alter_stmt() {
    auto token = lexer.next();
    std::shared_ptr<NodeAlterStmt> alter_stmt = std::make_shared<NodeAlterStmt>(token.position);

    assertTokenType(TokenType::ALTER, token.type, token.position);
    alter_stmt->_alter = std::make_shared<NodeAlter>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::TABLE, token.type, token.position);
    alter_stmt->_table = std::make_shared<NodeTable>(token.position);

    token = lexer.next();
    assertTokenType(TokenType::IDENTIFIER, token.type, token.position);
    alter_stmt->table_name = std::make_shared<NodeIdentifier>(token.position, token.value);

    token = lexer.next();
    assertTokenType(TokenType::PLUS, token.type, token.position);
    alter_stmt->_add = std::make_shared<NodeAdd>(token.position);

    alter_stmt->column_def = parse_column_def();

    return alter_stmt;
}

std::shared_ptr<Node> Parser::parse_value_list() {
    auto token = lexer.peek_next();
    std::shared_ptr<NodeValueList> value_list = std::make_shared<NodeValueList>(token.position);

    value_list->value_list.emplace_back(parse_literal());

    token = lexer.peek_next();
    while (token.type == TokenType::COMMA) {
        token = lexer.next();
        value_list->value_list.emplace_back(std::make_shared<NodeComma>(token.position));
        value_list->value_list.emplace_back(parse_literal());
        token = lexer.peek_next();
    }

    return value_list;
}

std::shared_ptr<Node> Parser::parse_column_list() {
    auto token = lexer.next();
    std::shared_ptr<NodeColumnList> column_list = std::make_shared<NodeColumnList>(token.position);

    assertTokenType(TokenType::IDENTIFIER, token.type, token.position);
    column_list->column_list.emplace_back(std::make_shared<NodeIdentifier>(token.position, token.value));

    token = lexer.peek_next();
    while (token.type == TokenType::COMMA) {
        token = lexer.next();
        column_list->column_list.emplace_back(std::make_shared<NodeComma>(token.position));
        token = lexer.next();
        assertTokenType(TokenType::IDENTIFIER, token.type, token.position);
        column_list->column_list.emplace_back(std::make_shared<NodeIdentifier>(token.position, token.value));
        token = lexer.peek_next();
    }

    return column_list;
}

std::shared_ptr<Node> Parser::parse_assignment_list() {
    auto token = lexer.peek_next();
    std::shared_ptr<NodeAssignmentList> assignment_list = std::make_shared<NodeAssignmentList>(token.position);

    assignment_list->assignment_list.emplace_back(parse_assignment_list_item());

    token = lexer.peek_next();
    while (token.type == TokenType::COMMA) {
        token = lexer.next();
        assignment_list->assignment_list.emplace_back(std::make_shared<NodeComma>(token.position));
        assignment_list->assignment_list.emplace_back(parse_assignment_list_item());
        token = lexer.peek_next();
    }

    return assignment_list;
}

std::shared_ptr<Node> Parser::parse_assignment_list_item() {
    auto token = lexer.next();
    std::shared_ptr<NodeAssignmentListItem> assignment_list_item = std::make_shared<NodeAssignmentListItem>(token.position);

    assertTokenType(TokenType::IDENTIFIER, token.type, token.position);
    assignment_list_item->identifier = std::make_shared<NodeIdentifier>(token.position, token.value);

    token = lexer.next();
    assertTokenType(TokenType::EQUAL, token.type, token.position);
    assignment_list_item->_eq = std::make_shared<NodeEqual>(token.position);

    assignment_list_item->expr = parse_literal();

    return assignment_list_item;
}

std::shared_ptr<Node> Parser::parse_column_def() {
    auto token = lexer.next();
    std::shared_ptr<NodeColumnDef> column_def = std::make_shared<NodeColumnDef>(token.position);

    assertTokenType(TokenType::IDENTIFIER, token.type, token.position);
    column_def->identifier = std::make_shared<NodeIdentifier>(token.position, token.value);

    token = lexer.next();
    if (token.type == TokenType::INTEGER) {
        column_def->data_type = std::make_shared<NodeInt>(token.position);
    } else if (token.type == TokenType::VARCHAR) {
        column_def->data_type = std::make_shared<NodeVarchar>(token.position);
    } else {
        throw std::runtime_error("Invalid data type");
    }

    return column_def;
}

std::shared_ptr<Node> Parser::parse_column_def_list() {
    auto token = lexer.next();
    std::shared_ptr<NodeColumnDef> column_def_list = std::make_shared<NodeColumnDef>(token.position);

    // TODO
    /*column_def_list->column_def = parse_column_def();

    token = lexer.next();
    if (token.type == TokenType::COMMA) {
        column_def_list->_comma = std::make_shared<NodeComma>(token.position);
        column_def_list->column_def_list = parse_column_def_list();
    } else {
        lexer.putback(token);
    }*/

    return column_def_list;
}

std::shared_ptr<Node> Parser::parse_literal() {
    auto token = lexer.next();
    
    switch (token.type)
    {
    case TokenType::INTEGER_LITERAL:
        return std::make_shared<NodeIntegerLiteral>(token.position, token.value);
        break;
    case TokenType::STRING_LITERAL:
        return std::make_shared<NodeStringLiteral>(token.position, token.value);
        break;
    case TokenType::FLOAT_LITERAL:
        return std::make_shared<NodeFloatLiteral>(token.position, token.value);
        break;
    default:
        throw std::runtime_error("Invalid literal");
        break;
    }
}