#include <stdio.h>
#include "foo.h"
#include "bar.h"
int main(int argc, char *argv[])
{
     printf("main +\n");
     func_foo();
     func_bar();
     printf("main -\n");
    return 0;
}
