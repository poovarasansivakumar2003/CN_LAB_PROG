BEGIN{
#include<stdio.h>
count=0
}
{
 if($1=="d")
 count++
}
END{
 printf("The Total no of Packets Dropped due toCongestion:%d ", count)
}
