#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
typedef unsigned int Position;

char l_name[5001];
char f_name[5001];
char birthday[5001];
int sex[101];
char address[5001];
char email[5001];
int id[101];
Position count=0;

int get_data(Position *count, char *l_name, char *f_name, char *birthday, int *sex, char *address, int *id, char *email)
{
	FILE *file = fopen("text.txt","r");
	if (file==NULL) return FALSE;
	while (fscanf(file, "%49[^:]:%49[^:]:%49[^:]:%d:%49[^:]:%u:%49s\n",
		l_name   +*count*50,
		f_name   +*count*50,
		birthday +*count*50,
		sex      +*count,
		address  +*count*50,
		id       +*count,
		email    +*count*50) == 7) (*count)++;
	fclose(file);
	return TRUE;
}

void show(Position count, char *l_name, char *f_name, char *birthday, int *sex, char*address, int id[], char *email)
{
	int i;
	for(i=0; i<count; i++)
	{
		printf("Họ và Tên: %s %s\n",l_name+i*50, f_name+i*50);
		printf("Giới tính: %s\n",(sex[i])?"Nam":"Nữ");
		printf("ID: %u\n",id[i]);
		printf("Địa chỉ: %s\n",address+i*50);
		printf("Email: %s\n",email+i*50);
		printf("-------------\n");
	}
}

int isFull(int count)
{
	return (count==100)?TRUE:FALSE;
}

int add(Position *count, char *l_name, char *f_name, char *birthday, int *sex, char *address, int *id, char *email)
{
	if (isFull(*count)) return FALSE;

	int e;
	printf("Nhập số lượng cần bổ xung : ");
	scanf("%d",&e);

	int i=0;
	while (e--)
	{
		i++;
		printf("Hãy nhập thông tin người thứ %d\n",i);
		printf("Họ lót: ");
		scanf(" %49[^\n]",l_name+*count*50);
		printf("Tên: ");
		scanf(" %49[^\n]",f_name+*count*50);
		printf("Sinh nhật: ");
		scanf(" %49[^\n]",birthday+*count*50);
		printf("Giới tính (Nam:1 | Nữ:0): ");
		scanf("%d",&sex[*count]);
		printf("Địa chỉ: ");
		scanf(" %49[^\n]",address+*count*50);

		id[*count] = -1;
		strcpy(email +*count*50,"none");

		(*count)++;
		if (isFull(*count)) return FALSE;
	}
	return TRUE;
}

void save(Position *count, char *l_name, char *f_name, char *birthday, int *sex, char *address, int *id, char *email)
{
	FILE *file = fopen("text.txt","w");
	int i;
	for(i=0; i<*count; i++)
	{

		fprintf(file,"%s:%s:%s:%d:%s:%d:%s\n",
		l_name   + i*50,
		f_name   + i*50,
		birthday + i*50,
		sex[i],
		address  + i*50,
		id[i],
		email    + i*50
		);
	}
	fclose(file);
}

int sort_list(Position *count, char *l_name, char *f_name, char *birthday, int *sex, char *address, int *id, char *email)
{
	return 1;
}

void menu()
{
	if (get_data(&count,l_name,f_name,birthday,sex,address,id,email)==FALSE)
	{
		printf("Không tìm thấy file dữ liệu!!!\n");
		return;
	}
	show(count,l_name,f_name,birthday,sex,address,id,email);
	printf("\n---\n");
	printf("Menu\n");
	printf("1-Thêm sinh viên\n");
	printf("2-Sắp xắp danh sách\n");
	printf("3-Xóa sinh viên\n");
	printf("4-Tìm sinh viên\n");
	printf("5-Cấp mã sinh viên\n");
	printf("6-Cấp email\n");
	printf("7-In ra danh sách\n");
	printf("8-Thoát\n");
	int select;
	while (TRUE)
	{
		printf("Nhap: ");
		scanf("%d",&select);
		if (select==8)
		{
			save(&count,l_name,f_name,birthday,sex,address,id,email); // lưu dữ liệu rồi dừng chương trình
			return;
		}
		if (select==1)
		{
			add(&count,l_name,f_name,birthday,sex,address,id,email);
			show(count,l_name,f_name,birthday,sex,address,id,email);
		}
		if (select==2)
		{
			sort_list(&count,l_name,f_name,birthday,sex,address,id,email);
		}
	}
}

int main()
{

	menu();

}