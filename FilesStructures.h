#include<stdio.h>
#include<string.h>
// total : 104857600
struct MasterBlock
{//44 size of master block :
	int file_bit_vector;//10 blocks : starts :44 till 1324 
	int blob_bit_vector;//420 blocks : starts :14124 till 67884
	int msg_bit_vector;//100 block : starts : 1324 till 14124
	int File_Meta_data_start;// starts : 67884
	int Message_Meta_data_start;//strats : 231724
	int Message_start;// starts : 11870124
	int BLob_data_start;//starts :  23521324
	int user_name_start;//starts : 23508524
	int user_num;
	int file_count;
	int pad;
};
struct file_counter
{
	int file_count;
};


/*
char *bit_vector = new char[128];
void *init_bit_vector(char *bit_vector,FILE *fp)
{
	for (int i = 0; i < 128; i++)
		bit_vector[i] = 0;
	fseek(fp, 20, SEEK_SET);
	fwrite(bit_vector, 100, 1, fp);
	return 0;


}
*/
struct user_name
{
	char u_name [124];
	int user_id;
};
struct File_Meta_data
{//total 1280 blocks : starts 67884:
	char user_name[56];
	char file_name[56];
	int message_start;
	int user_id;
	int offset;
	int pad;
};
struct Message_Meta_data
{
	char file_name[60];
	char user_name[56];
	int offset;
	int next;
	int pad;
};
struct Message
{
	char msg[124];
	int msg_id;
	
};
struct Blob_data
{
	char blob_d[120];
	int end;
	int next;
};