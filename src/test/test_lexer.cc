#include "lexer.hh"
#include "stdafx.hh"
using namespace std::literals;

auto data = R"(
#include <stdio.h>
#include "linking_list.h"

void pout(llist_int *list)
{
    for (int i = 0; i < list->length; ++i)
    {
        printf("index: %d\t data: %d\n", i, query_index_list_int(list, i));
    }
    puts("");
}

int main()
{
    llist_int *list = create_list_int(0);
    for (int i = 1; i < 10; ++i)
    {
        push_back_list_int(list, i);
    }
    pout(list);
    insert_index_list_int(list, 5, 999);
    insert_index_list_int(list, 3, 777);
    pout(list);
    delete_list_nod\
e_int(list, 8);
    delete_list_node_int(list, 8);
    pout(list);
    delete_list_int(list);
    return 0;
}
)"s;

auto main() -> int
{
    using std::cout, std::endl, std::format;
    auto ss    = std::istringstream{data};
    auto toker = simpc::lexer::tokenizer{ss};

    for (auto [t, i] : toker)
    // Range loop stops at eof or \0
    {
        if (i) cout << format("typeid: {0:i}\ttype: {0:n}\tinfo: \"{1}\"", t, *i);
        else cout << format("typeid: {0:i}\tsymbol: \"{0:n}\"", t);
        auto [line, col] = &toker;
        cout << format("\tline: {}\tcol: {}", line, col) << endl;
    }

    return 0;
}
