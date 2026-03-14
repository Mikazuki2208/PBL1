#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
typedef unsigned int Position;

struct sinhvien
{
	char name[50];
	unsigned int ID;
	char email[100];
};
typedef struct sinhvien SV;

struct ListInfo
{
	unsigned int capacity;
	SV *Array;
	unsigned int count;
};
typedef struct ListInfo *List;

List create(unsigned int capacity)
{
	List L = malloc(sizeof(struct ListInfo));
	if (L==NULL)
	{
		printf("Khong du bo nho");
		return NULL;
	}
	L->Array = malloc(sizeof(SV) * capacity);
	if (L->Array==NULL)
	{
		printf("Khong du bo nho");
		free(L);
		return NULL;
	}
	L->capacity = capacity;
	L->count = 0;
}

int isEmpty(List L)
{
	return L->count == 0;
}

int isFull(List L)
{
	return L->count == L->capacity;
}

int Insert(List L, SV a, Position p)
{
	if (isFull(L) || p<0 || p>L->count) return FALSE;
	Position count = L->count;
	while (p!=count)
	{
		L->Array[count]=L->Array[count-1];
		count--;
	}
	L->Array[p]=a;
	L->count++;
	return TRUE;
}

int Delete(List L, Position p)
{
	if (isEmpty(L) || p<0 || p>=L->count) return FALSE;
	Position count = L->count;
	while(p!=count)
	{
		L->Array[p]=L->Array[p+1];
		p++;
	}
	L->count--;
	return TRUE;
	
}

void ShowList(List L)
{
	if (isEmpty(L))
	{
		printf("List is Empty");
		return;
	}
	Position a;
	Position b = L->count;
	for(a=0 ; a<b; a++)
	{
		printf("name:%s\n",L->Array[a].name);
		printf("ID:%u\n",L->Array[a].ID);
		printf("email:%s\n",L->Array[a].email);
		printf("\n");
	}
}

int main()
{
	List Class = create(100);
	
	SV sv1 = {"Luong Vuong", 18, "luongvuongqn07@gmail.com"};
	SV sv2 = {"Nguyen Thi L",16, "nguyenthil102250264@dut.udn.vn"};
	SV sv3 = {"Banh Thi H" , 20, "banhthih@gmail.com"};
	
	Insert(Class, sv1, 0);
	Insert(Class, sv2, 1);
	Insert(Class, sv3, 2);
	
	int a = Delete(Class,2);
	
	ShowList(Class);
	
	if (a) printf("TRUE");
	else printf("FALSE");
}

