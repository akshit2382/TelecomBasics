#include<stdio.h>
#include<jansson.h>
#include<string.h>

void printData(json_t* obj)
{
	int i = 0, datalen;
	datalen = json_array_size(obj);
	char *first = NULL, *last = NULL;
	json_t *empl = NULL, *data = NULL;

	for(int i=0;i<datalen;i++)
	{
		empl = json_array_get(obj, i);

		data = json_object_get(empl, "firstname");
 		first = (char*) json_string_value(data);

		data = json_object_get(empl, "lastname");
		last = (char*) json_string_value(data);

		printf("%s %s\n",first,last);
	}
}

int main()
{
	json_t *obj = NULL;
	json_t *empl = NULL;
	json_error_t error;

	obj = json_load_file("employee.json", 0, &error);
	if(obj == NULL)
	{
		printf("Error in loading file\n");
		exit(1);
	}

	empl = json_object_get(obj, "employees");
	if(empl == NULL || json_array_size(empl)==0)
	{
		printf("No data found\n");
		exit(1);
	}
	printData(empl);
	return 0;
}
