BEGIN{
count=0
}
{
if($1=="d") #d stands for the packets drops.
count++
}
END{
printf("The Total no of Packets Dropped due to Congestion : %d\n\n", count)
}