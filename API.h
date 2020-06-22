#include"FilesStructures.h"
int get_bit_index(struct MasterBlock *mb,FILE *fp,int offset)
{
	fseek(fp, offset, SEEK_SET);
	int ind = 0;
	while (fgetc(fp) != 0)
	{
		ind++;
	}
	int pos = ftell(fp);
	fseek(fp, pos-1,SEEK_SET);
	char m = 1;
	fwrite(&m, 1, 1, fp);
	fflush(fp);
	return ind;

}
int get_blob_index(struct MasterBlock *mb, FILE *fp, int offset)
{
	fseek(fp, offset, SEEK_SET);
	int ind = 0;
	while (fgetc(fp) != 0)
	{
		ind++;
	}
	int pos = ftell(fp);
	fseek(fp, pos - 1, SEEK_SET);
	char m = 1;
	fwrite(&m, 1, 1, fp);
	fflush(fp);
	return ind;

}
int write_userfile_todata(FILE *fp,FILE *read_file,struct MasterBlock *mb,int blocks_req,int offset,int last_size)
{
	struct Blob_data *b_data = new Blob_data;
	int ind = 0;
	int temp = 0;
	while(blocks_req != 0)
	{
		temp = offset;
		fread(b_data->blob_d, 120, 1, read_file);
		b_data->end = 120;
		b_data->next = get_blob_index(mb, fp, mb->blob_bit_vector);
		fseek(fp, mb->BLob_data_start + (128 * temp), SEEK_SET);
		fwrite(b_data, 128, 1, fp);
		fflush(fp);
		offset = b_data->next;
		blocks_req--;


	}
	if (blocks_req == 0)
	{
		fread(b_data->blob_d, last_size, 1, read_file);
		b_data->end = last_size;
		b_data->blob_d[last_size] = '\0';
		b_data->next = NULL;
		fseek(fp, mb->BLob_data_start + (128 * offset), SEEK_SET);
		fwrite(b_data, 128, 1, fp);
		fflush(fp);
		return offset;
	}
	
	/*
	fread(b_data->blob_d, 120, 1, read_file);
	//fwrite(b_data->blob_d, 120, 1, fp);
	b_data->end = 120;
	b_data->next = get_blob_index(mb, fp, mb->blob_bit_vector);
	fseek(fp, mb->BLob_data_start + (128 * offset), SEEK_SET);
	//b_data->next = write_userfile_todata(fp, read_file, mb, blocks_req - 1, get_blob_index(mb,fp,mb->blob_bit_vector+offset),last_size);
	fseek(fp, mb->BLob_data_start + (128 * offset), SEEK_SET);
	fwrite(b_data, 128, 1, fp);
	return offset;
	*/

}
void download_file(char *file_name, struct MasterBlock *mb, FILE *fp)
{
	FILE *write_file;
	char extension[10];
	char file_name1[60];
	file_name1[0] = 'D';
	file_name1[1] = ':';
	file_name1[2] = '/';
	int j = 0;
	while (file_name[j] != '\0')
	{
		file_name1[3 + j] = file_name[j];
		j++;
	}
	file_name1[3 + j] = '.'; j++;
	file_name1[3 + j] = 'j'; j++;
	file_name1[3 + j] = 'p'; j++;
	file_name1[3 + j] = 'g'; j++;;

	file_name1[3+j] = '\0';
	write_file = fopen(file_name1, "wb+");
	struct Blob_data *bd = new struct Blob_data;
	struct File_Meta_data *fmd = new struct File_Meta_data;
	int offset = 0;
	int ind = 0;
	fseek(fp, mb->file_bit_vector + ind, SEEK_SET);
	int num_files = 0;
	num_files = mb->file_count;
	if (num_files == 0)
	{
		printf("\n No File, Go and Upload");
		return;
	}
	char ch;
	while (num_files != 0)
	{
		ch = fgetc(fp);
		if (ch == 1)
			num_files--;
		else if (ch == 0)
		{
			ind++;	continue;
		}

		fseek(fp, mb->File_Meta_data_start + (ind * 128), SEEK_SET);
		fread(fmd, 128, 1, fp);
		if (strcmp(fmd->file_name, file_name) == 0)
		{
			int i = 0;
			//while (fmd->)
			offset = fmd->offset;
			break;
		}
		ind++;
		fseek(fp, mb->file_bit_vector + ind, SEEK_SET);
	}
	if (offset == 0)
	{
		//printf("\n ")
	}
	
	fseek(fp, mb->BLob_data_start + (offset * 128), SEEK_SET);
	fread(bd, 128, 1, fp);
	while (bd->next != NULL)
	{
		
		fwrite(bd->blob_d, 120, 1, write_file);
		//fflush(write_file);
		offset = bd->next;
		if (offset != 0)
		{
			fseek(fp, mb->BLob_data_start + (offset * 128), SEEK_SET);
			fread(bd, 128, 1, fp);
		}
	}
	fwrite(bd->blob_d, bd->end, 1, write_file);
	printf("\n saved in you D drive : \n");

}
void write_message(char *file_name, struct MasterBlock *mb, FILE *fp,char * u_name)
{
	struct File_Meta_data *fmd = new struct File_Meta_data;

	//printf("\n File Name : \n");
	int ind = 0;

	fseek(fp, mb->file_bit_vector + ind, SEEK_SET);
	int num_files = 0;
	num_files = mb->file_count;
	if (num_files == 0)
	{
		printf("\n No File, Go and Upload");
		return;
	}
	char ch;
	while (num_files != 0)
	{
		ch = fgetc(fp);
		if (ch == 1)
			num_files--;
		else if (ch == 0)
		{
			ind++;	continue;
		}

		fseek(fp, mb->File_Meta_data_start + (ind * 128), SEEK_SET);
		fread(fmd, 128, 1, fp);
		if (strcmp(fmd->file_name, file_name) == 0)
		{
			break;
		}
		ind++;
		fseek(fp, mb->file_bit_vector + ind, SEEK_SET);

	}
	if (fmd->message_start == -1)
	{
		int offset = get_bit_index(mb, fp, mb->msg_bit_vector);
		fmd->message_start = offset;
		fseek(fp, mb->File_Meta_data_start + (ind * 128), SEEK_SET);
		fwrite(fmd, 128, 1, fp);
		fflush(fp);
		printf("\n Enter the comment : "); char buf;
		scanf("%c", &buf);
		struct Message_Meta_data *mmd = new struct Message_Meta_data;
		strcpy(mmd->file_name, file_name);
		strcpy(mmd->user_name, u_name);
		mmd->offset = offset;
		mmd->next = 0;
		fseek(fp, mb->Message_Meta_data_start + (128 * offset), SEEK_SET);
		fwrite(mmd, 128, 1, fp);
		fflush(fp);
		struct Message *msg = new struct Message;
		char msg1[124];
		scanf("%[^\n]s", msg1);
		strcpy(msg->msg, msg1);
		msg->msg_id = offset;
		fseek(fp, mb->Message_start + (offset * 128), SEEK_SET);
		fwrite(msg, 128, 1, fp);
		fflush(fp);
		
	}
	else
	{
		int next = fmd->message_start;
		struct Message_Meta_data *mmd2 = new struct Message_Meta_data;
		while (1)
		{
			fseek(fp, mb->Message_Meta_data_start + (next * 128), SEEK_SET);
			fread(mmd2, 128, 1, fp);
			if (mmd2->next == 0)
				break;
			next = mmd2->next;
		}
		mmd2->next = get_bit_index(mb, fp, mb->msg_bit_vector);
		fseek(fp, mb->Message_Meta_data_start + (next * 128),SEEK_SET);
		fwrite(mmd2, 128, 1, fp);
		fflush(fp);
		int offset = mmd2->next;
	//	fmd->message_start = offset;
		printf("\n Enter the comment : "); char buf;
		scanf("%c", &buf);
		struct Message_Meta_data *mmd = new struct Message_Meta_data;
		strcpy(mmd->file_name, file_name);
		strcpy(mmd->user_name, u_name);
		mmd->offset = offset;
		mmd->next = 0;
		fseek(fp, mb->Message_Meta_data_start + (128 * offset), SEEK_SET);
		fwrite(mmd, 128, 1, fp);
		fflush(fp);
		struct Message *msg = new struct Message;
		char msg1[124];
		scanf("%[^\n]s", msg1);
		strcpy(msg->msg, msg1);
		msg->msg_id = offset;
		fseek(fp, mb->Message_start + (offset * 128), SEEK_SET);
		fwrite(msg, 128, 1, fp);
		fflush(fp);
	}



	

}
void delete_message(struct MasterBlock *mb, int offset, struct File_Meta_data *fmd,int fmd_offset, FILE *fp)
{
	int delete_id = 0;
	printf("\n Enter id : ");
	scanf("%d", &delete_id);
	struct Message_Meta_data *mmd = new struct Message_Meta_data;
	struct Message_Meta_data *prev_mmd = new struct Message_Meta_data;
	int k=0;
	int prev_offset = 0;
	while (1)
	{
		fseek(fp, mb->Message_Meta_data_start + (offset * 128), SEEK_SET);
		fread(mmd, 128, 1, fp);
		if (mmd->offset == delete_id)
		{
			break;
		}
		k = 1;
		prev_offset = offset;
		fseek(fp, mb->Message_Meta_data_start + (offset * 128), SEEK_SET);
		offset = mmd->next;
		fread(prev_mmd, 128, 1, fp);
		if (offset == 0)
		{
			printf("\n What you may try can't work here. Enter Correct Id  Bro \n ");

		}
		//fseek(fp, mb->Message_Meta_data_start + (offset * 128), SEEK_SET);
	}
	if (k == 0)
	{
		if (mmd->next != 0)
			fmd->message_start = mmd->next;
		else
			fmd->message_start = -1;
		fseek(fp, mb->File_Meta_data_start + (128 * fmd_offset), SEEK_SET);
		fwrite(fmd, 128, 1, fp);
		fflush(fp);
	}
	prev_mmd->next = mmd->next;
	fseek(fp, mb->Message_Meta_data_start + (prev_offset * 128), SEEK_SET);
	fwrite(prev_mmd, 128, 1, fp);
	fflush(fp);
	fseek(fp, mb->msg_bit_vector + mmd->offset, SEEK_SET);
	int m = 1;
	fwrite(&m, 1, 1, fp);
	fflush(fp);
	printf("\n message Deleted .");



}
void read_messages(char *file_name, struct MasterBlock *mb, FILE *fp, char * u_name)
{
	struct File_Meta_data *fmd = new struct File_Meta_data;

	//printf("\n File Name : \n");
	int ind = 0;

	fseek(fp, mb->file_bit_vector + ind, SEEK_SET);
	int num_files = 0;
	num_files = mb->file_count;
	if (num_files == 0)
	{
		printf("\n No File, Go and Upload");
		return;
	}
	char ch;
	while (num_files != 0)
	{
		ch = fgetc(fp);
		if (ch == 1)
			num_files--;
		else if (ch == 0)
		{
			ind++;	continue;
		}

		fseek(fp, mb->File_Meta_data_start + (ind * 128), SEEK_SET);
		fread(fmd, 128, 1, fp);
		if (strcmp(file_name, fmd->file_name) == 0)
			break;
		ind++;
		fseek(fp, mb->file_bit_vector + ind, SEEK_SET);

	}
	int next = fmd->message_start;
	if (next == -1)
	{
		printf("\n Looks Like No Comments, Be the First one to Comment");
		return;
	}
	struct Message_Meta_data *mmd = new struct Message_Meta_data;
	fseek(fp, mb->Message_start + (128 * next), SEEK_SET);
	struct Message *msg = new struct Message;
	fread(msg, 128, 1, fp);
	//if (msg->)
	while (1)
	{
		fseek(fp, mb->Message_Meta_data_start + (128 * next), SEEK_SET);
		fread(mmd, 128, 1, fp);
		printf("\n user_name : "); puts(mmd->user_name);
		fseek(fp, mb->Message_start + (128 * next), SEEK_SET);
		fread(msg, 128, 1, fp);
		printf("\n id : %d .comment :  ",msg->msg_id); puts(msg->msg);
		next = mmd->next;
		if (next == 0)
			break;

	}
	int delete_id = 0;
	printf(" \n 1 : Delete Message \n 2 : Back  \n");
	//scanf("%d", &delete_id);
	scanf("%d", &delete_id);
	if (delete_id == 1)
	{
		delete_message(mb, fmd->message_start,fmd,ind,fp);
		read_messages(file_name, mb, fp, u_name);
	}
}
void delete_blob_bits(struct MasterBlock *mb, FILE *fp, struct File_Meta_data *fmd, int offset)
{
	struct Blob_data *bd = new Blob_data;
	fseek(fp, mb->blob_bit_vector + offset, SEEK_SET);
	int m = 0;
	fwrite(&m, 1, 1, fp);
	while (offset != 0)
	{
		fseek(fp, mb->BLob_data_start + (128 * offset), SEEK_SET);
		fread(bd, 128, 1, fp);
		offset = bd->next;
		if (offset == 0)
			break;
		fseek(fp, mb->blob_bit_vector + offset, SEEK_SET);
		int m = 0;
		fwrite(&m, 1, 1, fp);
		fflush(fp);
		

	}

}
void delete_file(char *file_name, struct MasterBlock *mb, FILE *fp)
{
	struct File_Meta_data *fmd = new struct File_Meta_data;

//	printf("\n File Name : \n");
	int ind = 0;

	fseek(fp, mb->file_bit_vector + ind, SEEK_SET);
	int num_files = mb->file_count;


	//int num_files = 0;
	num_files = mb->file_count;
	if (num_files == 0)
	{
		printf("\n No File, Go and Upload");
		return;
	}
	char ch;
	while (num_files != 0)
	{
		ch = fgetc(fp);
		if (ch == 1)
			num_files--;
		else if (ch == 0)
		{
			ind++;	continue;
		}

		fseek(fp, mb->File_Meta_data_start + (ind * 128), SEEK_SET);
		fread(fmd, 128, 1, fp);
		if (strcmp(file_name, fmd->file_name) == 0)
			break;
		ind++;
		fseek(fp, mb->file_bit_vector + ind, SEEK_SET);

	}
	int offset = fmd->offset;
	delete_blob_bits(mb, fp, fmd, offset);
	fseek(fp, mb->file_bit_vector + ind, SEEK_SET);
	int m = 0;
	fwrite(&m, 1, 1, fp);
	fflush(fp);
	mb->file_count--;
	fseek(fp, 0, SEEK_SET);
	fwrite(mb, 44, 1, fp);
	printf("\n File Deleted .");


}
void operation_onfile(char *file_name, struct MasterBlock *mb, FILE *fp,char *u_name)
{
	int option = 0;
	
	while (option != 5)
	{
		printf("\n 1: Enter Comment .\n 2: Read Comments .\n 3: Save File.\n 4: Delete  \n 5: Exit\n ");
		scanf("%d", &option);
		if (option == 1)
		{
			write_message(file_name, mb, fp, u_name);
		}
		else if (option == 2)
		{
			read_messages(file_name, mb, fp, u_name);

		}
		else if (option == 3)
		{
			download_file(file_name, mb, fp);
		}
		else if (option == 4)
		{
			delete_file(file_name, mb, fp);
			return;
		}
	}
}
int do_file_exists(struct MasterBlock *mb, FILE *fp, char *u_name,char *f_name)
{
	struct File_Meta_data *fmd = new struct File_Meta_data;

//	printf("\n File Name : \n");
	int ind = 0;

	fseek(fp, mb->file_bit_vector + ind, SEEK_SET);
	int num_files = 0;
	num_files = mb->file_count;
	if (num_files == 0)
	{
		//printf("\n No File, Go and Upload");
		return 1;
	}
	char ch;
	while (num_files != 0)
	{
		ch = fgetc(fp);
		if (ch == 1)
			num_files--;
		else if (ch == 0)
		{
			ind++;	continue;
		}

		fseek(fp, mb->File_Meta_data_start + (ind * 128), SEEK_SET);
		fread(fmd, 128, 1, fp);
		if (strcmp(fmd->file_name, f_name)==0)
			return 0;
		ind++;
		fseek(fp, mb->file_bit_vector + ind, SEEK_SET);

	}
	return 1;

}
void display_file_meta_data(struct MasterBlock *mb, FILE *fp,char *u_name)
{
	struct File_Meta_data *fmd = new struct File_Meta_data;

	printf("\n File Name : \n");
	int ind = 0;
	
		fseek(fp, mb->file_bit_vector +ind, SEEK_SET);
		int num_files = 0;
		num_files = mb->file_count;
		if (num_files == 0)
		{
			printf("\n No File, Go and Upload");
			return;
		}
		char ch;
		while (num_files!=0)
		{
			ch = fgetc(fp);
				if (ch == 1)
					num_files--;
				else if (ch == 0)
				{
					ind++;	continue;
				}
			
			fseek(fp, mb->File_Meta_data_start + (ind * 128), SEEK_SET);
			fread(fmd, 128, 1, fp);
			puts(fmd->file_name);
			ind++;
			fseek(fp, mb->file_bit_vector + ind, SEEK_SET);

		}
		printf("\n Enter the FIle you want : \n ");
		char file_name[50];
		char buff;
		scanf("%c", &buff);
		scanf("%[^\n]s", &file_name);
		operation_onfile(file_name, mb, fp,u_name);
		

}
void insert_file(char *u_name,struct MasterBlock *mb,FILE *fp)
{
	printf("Enter File Name : \n");
	char file_name[60];// = new char[60];
	char buff;
	scanf("%c", &buff);
	scanf("%[^\n]s", &file_name);
	if (do_file_exists(mb, fp, u_name, file_name)==0)
	{
		printf(" \n FIle name already present \n");
		insert_file(u_name, mb, fp);
		return;
	}

	printf("\n Enter Path : \n"); 
	char file_path[60];// = new char[60];
	scanf("%c", &buff);
	scanf("%[^\n]s", &file_path);
	//gets(file_path);
	FILE *read_file;
	read_file=fopen(file_path, "rb");
	if (read_file <= 0)
	{
		printf("\n Enter correct File \n");
		return;
	}
	mb->file_count++;
	fseek(fp, 0, SEEK_SET);
	fwrite(mb, 44, 1, fp);
	fflush(fp);
	int write_file_ind = get_bit_index(mb, fp,mb->file_bit_vector);
	fseek(fp, mb->File_Meta_data_start + (128 * write_file_ind), SEEK_SET);
	struct File_Meta_data *fmd= new struct File_Meta_data;
	strcpy(fmd->user_name,u_name);
	strcpy(fmd->file_name, file_name);
	fmd->message_start = -1;
	fmd->user_id = 0;//user id did not given 
	fseek(read_file, 0, SEEK_END);
	int read_size = ftell(read_file);
	int read_need_blocks = read_size / 120;
	int last_size = read_size % 120;
	int get_blob_ind = 0;

	fmd->offset = get_blob_index(mb,fp,mb->blob_bit_vector);

	fseek(fp, mb->File_Meta_data_start + (128 * write_file_ind), SEEK_SET);
	fwrite(fmd, 128, 1, fp);
	fflush(fp);
	fseek(read_file, 0, SEEK_SET);
	write_userfile_todata(fp, read_file, mb, read_need_blocks, fmd->offset, last_size);

	//struct Blob_data

}
void read_file(char *u_name, struct MasterBlock *mb, FILE *fp)
{
	display_file_meta_data( mb, fp,u_name);
}
void user_login(struct MasterBlock * mb,FILE *fp)
{
	char u_name[60];
	printf("\n Enter User Name : ");
	scanf("%s", &u_name);
	int option = 0;
	while (1)
	{
		printf("\n 1: INSERT FILE.\n 2: READ FILE.\n 3 : EXIT.  \n");
		scanf("%d", &option);
		if (option == 1)
			insert_file(u_name,mb,fp);
		else if (option == 2)
			read_file(u_name,mb,fp);
		else
			break;
	}

}