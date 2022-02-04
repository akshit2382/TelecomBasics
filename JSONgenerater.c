#include<stdio.h>
#include<jansson.h>

int main(int argc, char *argv[])
{
	if(argc <2) 
	{
		printf("please pass firstname and lastname!\n");
		return -1;
	}

	json_t* obj = json_object(), *data;
	json_t* arr = json_array();
	int itr = 1, counter =1, dataSize = 0;

	dataSize = argc;

	json_object_set_new(obj, "employees", arr);

	for(itr = 1; itr <= (dataSize /2); itr++)
	{
		data = json_object();
		json_array_append_new(arr, data);

		json_object_set_new(data, "firstname", json_string(argv[counter++]));

		json_object_set_new(data, "lastname", json_string(argv[counter++]));
	}
	
	int checkWrite = json_dump_file(obj, "sample.json", JSON_INDENT(4));

	if(checkWrite == 0)
		printf("success!\n");
	else
		printf("Error file cannot be dumped\n");

	return 0;
}
