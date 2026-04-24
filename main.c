#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define TRUE 1
#define FALSE 0
#define NOT_FOUND -1
#define MAX_SIZE 100
#define MAX_SV 100
#define MAX_CLASS 100

char l_name[MAX_SV*MAX_SIZE];
char f_name[MAX_SV*MAX_SIZE];
char birthday[MAX_SV*MAX_SIZE];
int sex[MAX_SIZE];
char address[MAX_SV*MAX_SIZE];
char email[MAX_SV*MAX_SIZE];
int id[MAX_SIZE];
int count=0;

int isFull()
{
	return (count==MAX_SV)?TRUE:FALSE;
}

int isEmpty()
{
	return (count==0)?TRUE:FALSE;
}

int get_data(char *name_file)
{
	FILE *file = fopen(name_file,"r");
	if (file==NULL) return FALSE;
	count =0;
	while (fscanf(file, "%99[^:]:%99[^:]:%99[^:]:%d:%99[^:]:%d:%99[^\n]\n",
		l_name   +count*MAX_SIZE,
		f_name   +count*MAX_SIZE,
		birthday +count*MAX_SIZE,
		sex      +count,
		address  +count*MAX_SIZE,
		id       +count,
		email    +count*MAX_SIZE) == 7) (count)++;
	fclose(file);
	return TRUE;
}

void show()
{
	printf("---Danh sách sinh viên---\n");
	if (isEmpty()) printf("Hiện chưa có thông tin.\n");
	int i;
	for(i=0; i<count; i++)
	{
		printf("   Họ và Tên: %s %s\n",l_name+i*MAX_SIZE, f_name+i*MAX_SIZE);
		printf("Giới tính: %s\n",(sex[i])?"Nam":"Nữ");
		if (id[i]==-1) printf("ID: Chưa có\n");
		else printf("ID: %d\n",id[i]);
		printf("Địa chỉ: %s\n",address+i*MAX_SIZE);
		printf("Email: %s\n",((strcmp(email+i*MAX_SIZE,"none")==0)?"Chưa có":email+i*MAX_SIZE));
	}
	printf("-------------\n");
	printf("\n");
}

void add()
{
	if (isFull())
	{
		printf("Đã đạt 100/100 người, không thể thêm!!!\n");
		return;
	}

	int e;
	printf("Nhập số lượng cần bổ xung : ");
	scanf("%d",&e);

	if (e+count >=100)
	{
		printf("Chỉ có thể thêm vào %d sinh viên!\n",100-count);
		return;
	}

	int i=0;
	while (e--)
	{
		i++;
		printf("Hãy nhập thông tin người thứ %d\n",i);
		printf("Họ lót: ");
		scanf(" %99[^\n]",l_name+count*MAX_SIZE);
		printf("Tên: ");
		scanf(" %99[^\n]",f_name+count*MAX_SIZE);
		printf("Sinh nhật (VD:02/09/1945): ");
		scanf(" %99[^\n]",birthday+count*MAX_SIZE);
		printf("Giới tính (Nam:1 | Nữ:0): ");
		scanf("%d",&sex[count]);
		printf("Địa chỉ : ");
		scanf(" %99[^\n]",address+count*MAX_SIZE);

		id[count] = -1;
		strcpy(email +count*MAX_SIZE,"none");

		count++;
		if (isFull())
		{
			printf("DDax dur 100 thanhf vieen, khong ther nhaapj theem\n");
			return;
		}
	}
}

void save(char *name_file)
{
	FILE *file = fopen(name_file,"w");
	if (file==NULL) return;
	int i;
	for(i=0; i<count; i++)
	{
		fprintf(file,"%s:%s:%s:%d:%s:%d:%s\n",
		l_name   + i*MAX_SIZE,
		f_name   + i*MAX_SIZE,
		birthday + i*MAX_SIZE,
		sex[i],
		address  + i*MAX_SIZE,
		id[i],
		email    + i*MAX_SIZE
		);
	}
	fclose(file);
}

void sort_list(int start, int stop)
{
	if (start>=stop) return;
	int prev = start -1;
	char full_name_select[MAX_SIZE*2];
	sprintf(full_name_select, "%s%s", l_name + stop*MAX_SIZE, f_name + stop*MAX_SIZE);
	int i;
	for (i=start; i<stop; i++) // chừa cái ptu cuối lại
	{
		char full_name_i[MAX_SIZE*2];
		sprintf(full_name_i, "%s%s", l_name + i*MAX_SIZE, f_name + i*MAX_SIZE);
		if (strcmp(full_name_i,full_name_select)<=0)// nếu name_i đứng trước name_select
		{
			prev++;
			char save[MAX_SIZE*2];

			strcpy(save, l_name+(i)*MAX_SIZE);
			strcpy(l_name+i*MAX_SIZE, l_name+(prev)*MAX_SIZE);
			strcpy(l_name+(prev)*MAX_SIZE, save);

			strcpy(save, f_name+(i)*MAX_SIZE);
			strcpy(f_name+i*MAX_SIZE, f_name+(prev)*MAX_SIZE);
			strcpy(f_name+(prev)*MAX_SIZE, save);

			strcpy(save, birthday+(i)*MAX_SIZE);
			strcpy(birthday+i*MAX_SIZE, birthday+(prev)*MAX_SIZE);
			strcpy(birthday+(prev)*MAX_SIZE, save);

			strcpy(save, address+(i)*MAX_SIZE);
			strcpy(address+i*MAX_SIZE, address+(prev)*MAX_SIZE);
			strcpy(address+(prev)*MAX_SIZE, save);

			strcpy(save, email+(i)*MAX_SIZE);
			strcpy(email+i*MAX_SIZE, email+(prev)*MAX_SIZE);
			strcpy(email+(prev)*MAX_SIZE, save);

			int save2;

			save2 = sex[prev];
			sex[prev] = sex[i];
			sex[i]    = save2;

			save2 = id[prev];
			id[prev] = id[i];
			id[i]    = save2;
		}
	}

	prev++;
	char save[MAX_SIZE*2];

	strcpy(save, l_name+(stop)*MAX_SIZE);
	strcpy(l_name+stop*MAX_SIZE, l_name+(prev)*MAX_SIZE);
	strcpy(l_name+(prev)*MAX_SIZE, save);

	strcpy(save, f_name+(stop)*MAX_SIZE);
	strcpy(f_name+stop*MAX_SIZE, f_name+(prev)*MAX_SIZE);
	strcpy(f_name+(prev)*MAX_SIZE, save);

	strcpy(save, birthday+(stop)*MAX_SIZE);
	strcpy(birthday+stop*MAX_SIZE, birthday+(prev)*MAX_SIZE);
	strcpy(birthday+(prev)*MAX_SIZE, save);

	strcpy(save, address+(stop)*MAX_SIZE);
	strcpy(address+stop*MAX_SIZE, address+(prev)*MAX_SIZE);
	strcpy(address+(prev)*MAX_SIZE, save);

	strcpy(save, email+(stop)*MAX_SIZE);
	strcpy(email+stop*MAX_SIZE, email+(prev)*MAX_SIZE);
	strcpy(email+(prev)*MAX_SIZE, save);

	int save2;

	save2 = sex[prev];
	sex[prev] = sex[stop];
	sex[stop]    = save2;

	save2 = id[prev];
	id[prev] = id[stop];
	id[stop]    = save2;

	sort_list(start,prev-1);
	sort_list(prev+1,stop);

}

void *find(char *name_find , int *n) 
{
	int i;
	int *list_find = (int *)calloc(MAX_SIZE, sizeof(int));
    for (i=0; i<count; i++) 
	{
		char full_name[MAX_SIZE*2];
		sprintf(full_name,"%s %s",l_name + i*MAX_SIZE, f_name + i*MAX_SIZE);
        if ( strcasestr(full_name, name_find) || strcasestr(l_name + i*MAX_SIZE, name_find) != NULL || strcasestr(f_name + i*MAX_SIZE, name_find) != NULL) 
		{
			list_find[(*n)++] = i;
        }
    }
	return list_find;
}

void find_by_name()
{
	char name_find[MAX_SIZE];
    printf("Nhập tên sinh viên: ");
    scanf(" %99[^\n]", name_find);

	int n=0 ,*list_name;
	list_name = find(name_find, &n);
	int i;

	printf("Những sinh viên có tên là %s gồm:\n",name_find);
	if (n == 0) printf("Không tìm thấy sinh viên có tên là: %s\n",name_find);
	else
	{
		for (i=0; i<n; i++)
		{
			printf("	%s %s\n",l_name + list_name[i]*MAX_SIZE, f_name + list_name[i]*MAX_SIZE);
		}
		printf("---\n\n");
	}
	
	free(list_name);
}

void find_and_delete()
{
	char name_find[MAX_SIZE];
    printf("Nhập tên sinh viên: ");
    scanf(" %99[^\n]", name_find);

	int n = 0, *list_find;
	list_find = find(name_find, &n);

	int i;
	printf("Sinh viên có tên là %s gồm: \n",name_find);
	
	for(i=0; i<n; i++)
	{
		printf("	Sinh Viên thứ %d\n",i+1);
		printf("Họ và Tên: %s %s\n",l_name+list_find[i]*MAX_SIZE, f_name+list_find[i]*MAX_SIZE);
		printf("Giới tính: %s\n",(sex[list_find[i]])?"Nam":"Nữ");
		if (id[list_find[i]]==-1) printf("ID: Chưa có\n");
		else printf("ID: %d\n",id[list_find[i]]);
		printf("Địa chỉ: %s\n",address+list_find[i]*MAX_SIZE);
		printf("Email: %s\n",((strcmp(email+list_find[i]*MAX_SIZE,"none")==0)?"Chưa có":email+list_find[i]*MAX_SIZE));
	}
	printf("---\n");
	printf("Bạn muốn xóa sinh sinh thứ mấy [-1: hủy xóa] :");
	int select_del ;
	scanf("%d",&select_del);
	select_del--;

	if (select_del<0 || select_del >= n)
	{
		printf("Đã hủy xóa.\n");
		free(list_find);
		return;
	}
	for (i=list_find[select_del]; i<count-1; i++)
	{
		strcpy(l_name+i*MAX_SIZE, l_name+(i+1)*MAX_SIZE);
		strcpy(f_name+i*MAX_SIZE, f_name+(i+1)*MAX_SIZE);
		strcpy(birthday+i*MAX_SIZE, birthday+(i+1)*MAX_SIZE);
		sex[i]= sex[i+1];
		strcpy(address+i*MAX_SIZE, address+(i+1)*MAX_SIZE);
		strcpy(email+i*MAX_SIZE, email+(i+1)*MAX_SIZE);
		id[i]= id[i+1];
	}
	count--;
	printf("Xóa thành công.\n");
	free(list_find);
}

void assign_id(char *name_file,int id_faculty)
{
	// kiểm tra có được sắp xếp hay chx
	int i;
	for (i=1; i<count; i++)
	{
		char full_name_i[MAX_SIZE*2];
		sprintf(full_name_i, "%s%s", l_name + i*MAX_SIZE, f_name + i*MAX_SIZE);

		char full_name_prev[MAX_SIZE*2];
		sprintf(full_name_prev, "%s%s", l_name + (i-1)*MAX_SIZE, f_name + (i-1)*MAX_SIZE);

		if (strcmp(full_name_prev,full_name_i) > 0)
		{
			printf("Danh sách sinh viên chưa được sắp xếp!!!\n");
			printf("Không thể cấp mã sinh viên!!!\n");
			return;
		}
	}

	// lấy mã lớn nhất
	int id_max = 0;
	for (i=0;i<count;i++)
	{
		id_max = (id[i]>id_max)?id[i]:id_max;
	}

	id_max = id_max%10000; // lấy 4 số cuối

	// cấp mã ID
	for (i=0;i<count;i++)
	{
		int year_class;
		sscanf(name_file, "class/%2d", &year_class);
		if (id[i]==-1) id[i] = id_faculty*1000000 + year_class*10000 + (++id_max);
	}
}

void assign_email()
{
	int i;
	for (i=0; i<count; i++)
	{
		if (id[i]==-1)
		{
			printf("Sinh viên %s %s chưa được cấp mã sv\n",l_name + i*MAX_SIZE, f_name + i*MAX_SIZE);
		}
		else
		{
			sprintf(email+i*MAX_SIZE, "%d@sv1.dut.udn.vn",id[i]);
		}
	}
}

void menu(char *name_file, int id_faculty);

char name_class[MAX_CLASS*MAX_SIZE];
int faculty_class[MAX_CLASS];
int count_class;
void get_class()
{
	FILE *file = fopen("name_class.txt","r");
	if (file==NULL) return;

	count_class =0;
	while (fscanf(file, "%99[^:]:%d\n",
		name_class+count_class*MAX_SIZE,
		faculty_class+count_class) == 2) (count_class)++;
	fclose(file);
}

void select_create_class();

int main()
{
	get_class();
	select_create_class();
}

void select_create_class()
{
	while (TRUE)
	{
		printf("---Menu---\n");
		printf("1-Chọn lớp\n");
		printf("2-Thêm lớp\n");
		printf("3-Thoát\n");
		printf("\n");

		int select_1;
		printf("Nhập: ");
		scanf("%d",&select_1);
		if (select_1==1)
		{
			printf("---Danh Sách Các Lớp Hiện Tại---\n");
			if (count_class==0)
			{
				printf("Hiện chưa có lớp nào cả!!!");
				continue;
			}
			int i;
			for (i=0;i<count_class;i++)
			{
				printf("%d: %s\n",i+1,name_class+i*MAX_SIZE);
			}
			printf("\n");
			printf("Nhập: ");
			int select_2;
			scanf("%d",&select_2);
			
			if (select_2<1 || select_2>count_class)
			{
				printf("Số thứ tự không hợp lệ !!!");
				continue;
			}
			char name_file[MAX_SIZE+4];// vì .txt có 4 ký tự :>
			sprintf(name_file,"class/%s.txt",name_class+(select_2-1)*MAX_SIZE);
			menu(name_file, faculty_class[select_2-1]);
		}
		else if (select_1==2)
		{
			printf("---Bổ xung lớp---\n");
			printf("\n");

			char name_class_update[MAX_SIZE];
			int faculty_class_update;
			printf("Nhập mã khoa: ");
			scanf("%d",&faculty_class_update);
			printf("Nhập tên lớp: ");
			scanf("%s",name_class_update);

			// tạo file/cập nhật tên vào name_class.txt
			char name_file[MAX_SIZE+4];
			sprintf(name_file,"class/%s.txt",name_class_update);
			FILE *file_1 = fopen(name_file,"w");
			fclose(file_1);

			FILE *file_2 = fopen("name_class.txt","a");
			fprintf(file_2,"%s:%d\n",name_class_update,faculty_class_update);
			fclose(file_2);
			// cập nhật danh sách hiện có
			strcpy(name_class + count_class*MAX_SIZE, name_class_update);
			faculty_class[count_class]= faculty_class_update;
			count_class++;

		}
		else if (select_1==3) return;
		else
		{
			printf("Không hợp lệ!!!Hãy nhập lại!!!\n");
		}
	}
}

void menu(char *name_file, int id_faculty)
{
	if (get_data(name_file)==FALSE)
	{
		printf("Không tìm thấy file dữ liệu!!!\n");
		return;
	}
	int select;
	while (TRUE)
	{
		printf("---Menu---\n");
		printf("1-Thêm sinh viên\n");
		printf("2-Sắp xắp danh sách\n");
		printf("3-Xóa sinh viên\n");
		printf("4-Tìm sinh viên\n");
		printf("5-Cấp mã sinh viên\n");
		printf("6-Cấp email\n");
		printf("7-In ra danh sách\n");
		printf("8-Thoát\n");
		printf("\n");

		printf("Nhap: ");
		scanf("%d",&select);
		if (select==1)
		{
			add();
			show();
		}
		else if (select==2)
		{
			if (count>0)
			{
				sort_list(0,count-1);
				printf("Đã sắp xếp xong!\n");
			}
			else
			{
				printf("Danh sách rỗng!\n");
			}
		}
		else if (select==3)
		{
			find_and_delete();
		}
		else if (select==4)
		{
			find_by_name();
		}
		else if (select==5)
		{
			assign_id(name_file,id_faculty);
		}
		else if (select==6)
		{
			assign_email();
		}
		else if (select==7)
		{
			show();
		}
		else if (select==8)
		{
			save(name_file); // lưu dữ liệu rồi dừng chương trình
			return;
		}
		else
		{
			printf("Không hợp lệ!!!Hãy nhập lại!!!\n");
		}
	}
}
