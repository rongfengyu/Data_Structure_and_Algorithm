#include <stdio.h>
#include "foo.h"
#include "bar.h"
#include "play.h"
int main(int argc, char *argv[])
{
     printf("main +\n");
     func_foo();
     func_bar();
     func_play();
     printf("main -\n");
    return 0;
}
