#include<stdio.h>
#include<stdlib.h>

int main()
{
	int left,right;
	int sum;
	int given = 15;
	int count = 0;
	
	for (sum=0, right=1; sum<given; sum +=right, right++)
	;
	printf("\nright =  %d", right);
	
	for(left=1, right--; left<= given/2;)
	if(sum>given)
		sum -=(left++);
	else
	{
		if(sum == given)
		{
		printf("\n%d = sum from %d to %d", given, left, right);
		count++;
		}
		sum +=(++right);
	}
	
	if(count>0)
		printf("\nthere are %d solutions", count);
	else
	    printf("\nno solutions");
}



