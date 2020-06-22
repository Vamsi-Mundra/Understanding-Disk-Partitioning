#include<stdio.h>
#include"API.h"
struct file_counter *fc = new struct file_counter;
	
int main()
{
	fc->file_count = 104857652;
	
	struct MasterBlock *MD = new struct MasterBlock;
	
	//MD->file_count = 0;
	FILE *fp;
	fp = fopen("instagram.bin", "rb+");
	fseek(fp, 0, SEEK_SET);
	fread(MD, 44, 1, fp);
	if (MD->file_count <= 0)
		MD->file_count = 0;
	MD->file_bit_vector = 44;
	MD->msg_bit_vector = 1324;
	MD->blob_bit_vector = 14124;
	MD->File_Meta_data_start = 67884;
	MD->Message_Meta_data_start = 231724;
	MD->Message_start = 11870124;
	MD->user_name_start = 235085524;
	MD->BLob_data_start = 283796;
	fwrite(MD, 44, 1, fp);

	user_login(MD,fp);
}