#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define NOT_FOUND -1
#define MAX_SIZE 50
#define MAX_SV 100

typedef unsigned int Position;
typedef unsigned int Quantity;

char l_name[MAX_SV*MAX_SIZE];
char f_name[MAX_SV*MAX_SIZE];
char birthday[MAX_SV*MAX_SIZE];
int sex[MAX_SIZE];
char address[MAX_SV*100];
char email[MAX_SV*MAX_SIZE];
int id[MAX_SIZE];
Position count=0;

int isFull(Quantity count)
{
	return (count==MAX_SV)?TRUE:FALSE;
}

int isEmpty(Quantity count)
{
	return (count==0)?TRUE:FALSE;
}

int get_data(Position *count, char *l_name, char *f_name, char *birthday, int *sex, char *address, int *id, char *email)
{
	FILE *file = fopen("text.txt","r");
	if (file==NULL) return FALSE;
	while (fscanf(file, "%49[^:]:%49[^:]:%49[^:]:%d:%99[^:]:%d:%49s\n",
		l_name   +*count*MAX_SIZE,
		f_name   +*count*MAX_SIZE,
		birthday +*count*MAX_SIZE,
		sex      +*count,
		address  +*count*100,
		id       +*count,
		email    +*count*MAX_SIZE) == 7) (*count)++;
	fclose(file);
	return TRUE;
}

void show(Quantity count, char *l_name, char *f_name, char *birthday, int *sex, char*address, int id[], char *email)
{
	if (isEmpty(count)) printf("Hiện chưa có thông tin.\n");
	int i;
	for(i=0; i<count; i++)
	{
		printf("Họ và Tên: %s %s\n",l_name+i*MAX_SIZE, f_name+i*MAX_SIZE);
		printf("Giới tính: %s\n",(sex[i])?"Nam":"Nữ");
		if (id[i]==-1) printf("ID: Chưa có\n");
		else printf("ID: %d\n",id[i]);
		printf("Địa chỉ: %s\n",address+i*100);
		printf("Email: %s\n",((strcmp(email+i*MAX_SIZE,"none")==0)?"Chưa có":email+i*MAX_SIZE));
	}
	printf("-------------\n");
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
		scanf(" %49[^\n]",l_name+*count*MAX_SIZE);
		printf("Tên: ");
		scanf(" %49[^\n]",f_name+*count*MAX_SIZE);
		printf("Sinh nhật (VD:02/09/1945): ");
		scanf(" %49[^\n]",birthday+*count*MAX_SIZE);
		printf("Giới tính (Nam:1 | Nữ:0): ");
		scanf("%d",&sex[*count]);
		printf("Địa chỉ : ");
		scanf(" %49[^\n]",address+*count*100);

		id[*count] = -1;
		strcpy(email +*count*MAX_SIZE,"none");

		(*count)++;
		if (isFull(*count)) return FALSE;
	}
	return TRUE;
}

void save(Quantity *count, char *l_name, char *f_name, char *birthday, int *sex, char *address, int *id, char *email)
{
	FILE *file = fopen("text.txt","w");
	int i;
	for(i=0; i<*count; i++)
	{
		fprintf(file,"%s:%s:%s:%d:%s:%d:%s\n",
		l_name   + i*MAX_SIZE,
		f_name   + i*MAX_SIZE,
		birthday + i*MAX_SIZE,
		sex[i],
		address  + i*100,
		id[i],
		email    + i*MAX_SIZE
		);
	}
	fclose(file);
}

int sort_list(Quantity *count, char *l_name, char *f_name, char *birthday, int *sex, char *address, int *id, char *email)
{
	return 1;
}

void menu()
{
	if (get_data(&count,l_name,f_name,birthday,sex,address,id,email)==FALSE)
	{
		printf("Không tìm thấy file dữ liệu!!!\n");
	}
	show(count,l_name,f_name,birthday,sex,address,id,email);
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
		if (select==4)
		{
			return;
		}
	}
}

int main()
{

	menu();

}
