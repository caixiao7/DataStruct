#include <iostream>
int next[1024] = { -1 };

void MyNext(char *sub)
{
	int len = strlen(sub);
	int i = 0;
	int k = -1;
	next[0] = -1;

	while (i < len - 1)
	{
		if (k == -1 || sub[i] == sub[k])
		{
			k++;
			i++;
			next[i] = k;
		}
		else
			k = next[k];
	}

}


int MyKMP(char *src, char *sub)
{
	int i = 0;
	int j = 0;
	int src_len = strlen(src);
	int sub_len = strlen(sub);
	while (i < src_len && j < sub_len)
	{
		if (j == -1 || src[i] == sub[j])
		{
			i++;
			j++;
		}
		else
			j = next[j];
	}
	if (j >= sub_len)
		return i - j;
	else
		return -1;
}