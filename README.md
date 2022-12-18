# LambdaDB

## Build

```bash
mkdir build && cd build
cmake ..
make
```

## Run

```bash
./lambdadb
```

## Install AWS SDK
    
```bash
git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp
mkdir sdk_build
cd sdk_build
cmake ../aws-sdk-cpp -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/usr/local/ -DCMAKE_INSTALL_PREFIX=/usr/local/ -DBUILD_ONLY="s3"
make
make install
```

## SQL Grammar
    
```ts
stmt = select_stmt | insert_stmt | delete_stmt | update_stmt | create_stmt | drop_stmt | alter_stmt;
select_stmt = "SELECT" select_list "FROM" table_list [join_clause] [where_clause] [order_by_clause] [limit_clause];
select_list = "*" | select_list_item {"," select_list_item};
select_list_item = expr ["AS" ident];
insert_stmt = "INSERT INTO" table_name ["(" column_list ")"] "VALUES" "(" expr_list ")";
delete_stmt = "DELETE FROM" table_name [where_clause];
update_stmt = "UPDATE" table_name "SET" update_list [where_clause];
update_list = update_list_item {"," update_list_item};
update_list_item = column_name "=" expr;
create_stmt = "CREATE TABLE" table_name "(" column_def {"," column_def} ")";
column_def = column_name column_type [column_constraint];
column_type = "INT" | "VARCHAR";
column_constraint = "PRIMARY KEY" | "NOT NULL";
drop_stmt = "DROP TABLE" table_name;
alter_stmt = "ALTER TABLE" table_name "ADD" column_def;
join_clause = join_type "JOIN" table_name "ON" expr;
join_type = "INNER" | "LEFT" | "RIGHT" | "FULL";
table_list = table_name {"," table_name};
table_name = ident ident;
where_clause = "WHERE" expr;
order_by_clause = "ORDER BY" order_by_list;
order_by_list = order_by_item {"," order_by_item};
order_by_item = expr ["ASC" | "DESC"];
limit_clause = "LIMIT" expr;

# TODO: Extract AND and OR to a new rule
expr = expr ("=" | "<>" | "<" | "<=" | ">" | ">=" | "AND" | "OR" | "LIKE") expr
unary_expr = ("+" | "-" | "NOT") primary_expr;
primary_expr = literal_value | ident | ident "." ident | "(" expr ")";

ident = [a-zA-Z_][a-zA-Z0-9_]*;
literal_value = [0-9]+ | [0-9]*.[0-9]+ | "'" [^']* "'" | "NULL";
```