#include <stdio.h>
#include "cJSON.h"

int make_json();
int parse_json();
char* filename="demo.json";

int main()
{
		make_json();

		parse_json();
}
int parse_json()
{
	FILE* fp = NULL;
	char data[1024] = {0};
	fp = fopen (filename, "r");
	if(fp == NULL)
	{
		return -1;
	}
	fread (data, sizeof(data), 1, fp);
	
	cJSON *root, *item;
	root = cJSON_Parse(data);
	
	item = cJSON_GetObjectItem(root,"name");
	if(item) 
	{
		printf ("%s:%s\n", item->string, item->valuestring);
	}
	
	item=cJSON_GetObjectItem(root,"age");
	if(item) 
	{
		printf("%s:%d\n", item->string, item->valueint);
	}
	item=cJSON_GetObjectItem(root,"details");
	item=cJSON_GetObjectItem(root,"skills");
	if(item)
	{
		int i;
		int count=cJSON_GetArraySize(item);
		printf ("%s:\t", item->string);
		for(i=0; i<count; i++)
		{
			cJSON* t=cJSON_GetArrayItem(item,i);
			if (i == count-1)
			{
				printf ("%d\t", t->valueint);
			}
			else
			{
				printf ("%s\t", t->valuestring);
			}
		}
		printf ("\n");
	}
	fclose (fp);
	return 0;
}


int make_json()
{
	cJSON* root, *detail, *array;

	FILE * fp=NULL;
	
	root=cJSON_CreateObject();
	detail=cJSON_CreateObject();
	array=cJSON_CreateArray();
	
	if(root==NULL || detail==NULL || array==NULL)
	{
		printf("Error\n");
		return -1;
	}
	cJSON_AddItemToObject(root, "name", cJSON_CreateString("张三"));	//添加字符串或者对象
	cJSON_AddStringToObject(root,"school","北京大学");
	cJSON_AddItemToObject(root,"height",cJSON_CreateNumber(1.72));
	cJSON_AddNumberToObject(root, "age",22);	//添加数字

	//嵌套对象
	cJSON_AddItemToObject(root,"details",detail);	
	cJSON_AddNullToObject(detail,"a");
	cJSON_AddFalseToObject(detail,"b");
	cJSON_AddStringToObject(detail,"c","***********");
	cJSON_AddBoolToObject(detail,"d",8);


	int tt[6]={1,2,3,4,5,6};
	char* mm[6]={"111","222","333","444","555","666"}; 
	cJSON_AddItemToObject(detail,"e",cJSON_CreateIntArray(tt,6));
	cJSON_AddItemToObject(detail,"f",cJSON_CreateStringArray((const char **)mm,6));
	//嵌套数组
	cJSON_AddItemToObject(root,"skills",array);
	cJSON_AddItemToArray(array,cJSON_CreateString("Linux"));
	cJSON_AddItemToArray(array,cJSON_CreateString("服务端"));
	cJSON_AddItemToArray(array,cJSON_CreateString("C语言"));
	cJSON_AddItemToArray(array,cJSON_CreateNumber(26));
	
	puts(cJSON_Print(root));
	fp=fopen(filename,"w");
	if (fp == NULL)
	{
		return -1;
	}
	fwrite (cJSON_Print(root), sizeof(char), strlen (cJSON_Print(root)), fp);
	fclose (fp);

	cJSON_Delete(root);
	
	printf("MakeJson END.............\n\n");
	return 0;
}
