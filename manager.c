// nix-shell -p gcc ncurses
// gcc main.c manager.c -o main -lncursesw

#include "manager.h"
#include <locale.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // dùng để tạm dừng 1 lúc

char l_name[MAX_SV*MAX_SIZE];
char f_name[MAX_SV*MAX_SIZE];
char birthday[MAX_SV*MAX_SIZE];
int sex[MAX_SV];
char address[MAX_SV*MAX_SIZE];
char email[MAX_SV*MAX_SIZE];
int id[MAX_SV];
int count=0;

char name_class[MAX_CLASS*MAX_SIZE];
char name_file[MAX_SIZE+20];// vì 'class/' +maLop+ '.txt'
int faculty_class[MAX_CLASS];
int count_class;
int is_valid_input;
int is_data_loaded;

// {giao diện
int y;
int current_class;
int event;
int page = 0;
int select_choice   =-1;
int highlight=0;
int run=TRUE;
int print = FALSE;
char text[999];

int n_page0 = 3;
char *choices_page0[] = {
	"Chọn lớp",
	"Quản lý",
	"Thoát"
};

int n_page1 = 2;
char *choices_page1[] = {
	"Danh sách lớp",
	"Quay lại"
};

int n_page2 = 3;
char *choices_page2[] = {
	"Thêm Lớp",
	"Xóa Lớp",
	"Thoát"
};

int n_menu = 8;
char *choices_menu[] = {
	"Thêm sinh viên",
	"Sắp xếp danh sách",
	"Xóa sinh viên",
	"Tìm sinh viên",
	"Cấp mã sinh viên",
	"Cấp email",
	"In ra danh sách",
	"Thoát"
};

// giao diện}

int isFull() {return (count==MAX_SV)?TRUE:FALSE;}

int isEmpty() {return (count==0)?TRUE:FALSE;}

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
	clear();
	box(stdscr,0,0);
	y=1;
	strcpy(text,"\u300CDanh Sách Sinh Viên\u300D");
	int col = (COLS-strlen(text))/2;
	mvprintw(y++,col,"%s",text);

	if (isEmpty())
	{
		strcpy(text,"Hiện chưa có thông tin.");
		col = (COLS - strlen(text))/2;
		mvprintw(y++,col,"%s",text);
		getch();
		return;
	}
	int i=0;
	while (TRUE)
	{
		int e;
		for (e=0;e<=8;e++)
		{
			move(y+e,0);
			clrtoeol();
		}

		mvprintw(y,3,"Họ và Tên: %s %s",l_name+i*MAX_SIZE, f_name+i*MAX_SIZE);
		mvprintw(y+1,3,"Giới tính: %s",(sex[i])?"Nam":"Nữ");
		if (id[i]==-1) mvprintw(y+2,3,"ID: Chưa có");
		else mvprintw(y+2,3,"ID: %d",id[i]);
		mvprintw(y+3,3,"Địa chỉ: %s",address+i*MAX_SIZE);
		mvprintw(y+4,3,"Email: %s",((strcmp(email+i*MAX_SIZE,"none")==0)?"Chưa có":email+i*MAX_SIZE));

		strcpy(text,"person number");
		col = (COLS - strlen(text))/2;
		mvprintw(y+5,col,"%s %d/%d",text,i+1,count);

		strcpy(text,"prev");
		col = COLS/6;
		mvprintw(y+7,col,"%s",text);
		strcpy(text,"[<-]");
		col = COLS/6 + (strlen("prev")-strlen("[<-]"))/2;
		mvprintw(y+8,col,"%s",text);

		strcpy(text,"exit");
		col = (COLS - strlen(text))/2;
		mvprintw(y+7,col,"%s",text);
		strcpy(text,"[Esc]");
		col = (COLS - strlen(text))/2;
		mvprintw(y+8,col,"%s",text);

		strcpy(text,"next");
		col =(COLS-COLS/6) - (strlen("next")/2);
		mvprintw(y+7,col,"%s",text);
		strcpy(text,"[->]");
		col = (COLS-COLS/6) - (strlen("next")-strlen("[->]"))/2;
		mvprintw(y+8,col,"%s",text);

		box(stdscr,0,0);
		refresh();

		event = getch();
		switch(event)
		{
			case KEY_LEFT:
			{
				i--;
				if (i<0) i=count-1;
				break;
			}

			case KEY_RIGHT:
			{
				i++;
				if (i==count) i=0;
				break;
			}

			case 27: // Nút Esc
			{
				return;
			}
		}
	}
}

int check_int(char *text, int *res)
{
	char *end;
	long value = strtol(text,&end,10); // chuyển về hệ số 10, end sẽ đọc đến khi gặp ký tự != số
	if (*end != '\0' || end == text) // nếu strtol trả thấy sai ngay từ đầu thì địa chỉ trả về là text
	{
		return FALSE;
	}
	*res = (int)value;
	return TRUE;
}

int check_birthday(char *text)
{
	char birth[MAX_SIZE];
	int d, m, y;
	int d_max[13] = {000,31,28,31,30,31,30,31,31,30,31,30,31};
	if (sscanf(text, "%d/%d/%d", &d, &m, &y) == 3) {
		// năm nhuận
		if ((y%4==0 && y%100!=0) || (y%400==0))
		{
			d_max[2]=29;
		}
		if (1<=m && m<=12) // tháng thỏa mãn
		{
			if (1<=d && d<=d_max[m]) // ngày thỏa mãn
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

int check_name_class(char *text)
{
	int year;
	char name[MAX_SIZE];
	if (sscanf(text,"%2d%s",&year,name) == 2) return TRUE;
	return FALSE;
}

void add()
{
	clear();
	box(stdscr,0,0);
	y = 1;

	if (isFull())
	{
		strcpy(text,"Đã đạt 100/100 người, không thể thêm!!!");
		int col = (COLS - strlen(text))/2;
		mvprintw(y,col,"%s",text);
		refresh();
		getch();
		page = 3;
		return;
	}

	strcpy(text,"\u300CThêm Sinh Viên\u300D");
	int col = (COLS - strlen(text))/2;
	mvprintw(y++,col,"%s",text);
	refresh();

	char inp[MAX_SIZE];
	is_valid_input = FALSE;
	int e;
	curs_set(1); // con trỏ đứng nháy nháy
	echo();      // hiện lên những gì nhập vào màn hình
	while (!is_valid_input)
	{
		is_valid_input = TRUE;
		mvprintw(y, 5, "Nhập số lượng sinh viên cần thêm: ");
		mvgetnstr(y, 40, inp, MAX_SIZE - 1);
		is_valid_input = check_int(inp,&e);
		if (!is_valid_input || e<0)
		{
			is_valid_input = FALSE;
			move(y,40);
			clrtoeol(); // xóa dữ liệu từ vị trí con trỏ đến hết dòng
			box(stdscr,0,0);
			mvprintw(y+1,5,"Vui lòng nhập số nguyên dương !!!");
		}
	}
	move(++y,5);
	clrtoeol();
	box(stdscr,0,0);
	
	if (e+count > MAX_SV)
	{
		mvprintw(y++,5,"Chỉ có thể thêm vào %d sinh viên!",MAX_SV-count);
		noecho();    // tắt những gì đc nhập vào màn hình
		curs_set(0); // tắt con trỏ đứng
		refresh();
		getch();
		page = 3;
		return;
	}
	
	int i=1;
	while (e--)
	{
		clear();
		y = 1;
		box(stdscr,0,0);

		mvprintw(y++,5,"Hãy nhập thông tin người thứ %d",i++);
		
		mvprintw(y,5,"Họ lót: ");
		mvgetnstr(y++, 40, l_name+count*MAX_SIZE, MAX_SIZE - 1);
		
		mvprintw(y,5,"Tên: ");
		mvgetnstr(y++, 40, f_name+count*MAX_SIZE, MAX_SIZE - 1);
		
		is_valid_input = FALSE;
		while (!is_valid_input)
		{
			mvprintw(y,5,"Sinh nhật (VD:02/09/1945): ");
			mvgetnstr(y, 40, inp, MAX_SIZE - 1);
			is_valid_input = check_birthday(inp);
			if (!is_valid_input)
			{
				move(y,40);
				clrtoeol();
				box(stdscr,0,0);
				mvprintw(y+1,5,"Không hợp lệ, vui lòng nhập lại !!!");
			}
		}
		strcpy(birthday+count*MAX_SIZE,inp);
		move(++y,5);
		clrtoeol();
		box(stdscr,0,0);
		
		int val;
		is_valid_input = FALSE;
		while (!is_valid_input)
		{
			mvprintw(y,5,"Giới tính (Nam:1 | Nữ:0): ");
			mvgetnstr(y, 40, inp, MAX_SIZE - 1);
			is_valid_input = check_int(inp,&val);
			if (is_valid_input==FALSE || (val!=0 && val!=1))
			{
				move(y,40);
				clrtoeol();
				box(stdscr,0,0);
				mvprintw(y+1,5,"Hãy chọn lại !!! (Nam:1 | Nữ:0)");
				is_valid_input = FALSE;
			}
		}
		sex[count] = val;
		move(++y,5);
		clrtoeol();
		box(stdscr,0,0);

		mvprintw(y,5,"Địa chỉ : ");
		mvgetnstr(y++, 40, address+count*MAX_SIZE, MAX_SIZE - 1);
		
		id[count] = -1;
		strcpy(email +count*MAX_SIZE,"none");
		
		count++;

		if (isFull())
		{
			mvprintw(y++,5,"Đã đủ 100 sinh viên");
			break;
		}
	}
	mvprintw(y++,5,"Hoàn thành bổ xung sinh viên!");

	noecho();    // tắt những gì đc nhập vào màn hình
	curs_set(0); // tắt con trỏ đứng

	getch(); // để nó dừng
	page = 3;
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

void sort_list(int start, int stop)// quicksort
{
	if (start>=stop) return;
	int prev = start -1;
	char full_name_select[MAX_SIZE*2];
	sprintf(full_name_select, "%s %s", l_name + stop*MAX_SIZE, f_name + stop*MAX_SIZE);
	int i;
	for (i=start; i<stop; i++) // chừa cái ptu cuối lại
	{
		char full_name_i[MAX_SIZE*2];
		sprintf(full_name_i, "%s %s", l_name + i*MAX_SIZE, f_name + i*MAX_SIZE);
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

int *find(char *name_find , int *n) 
{
	int i;
	int *list_find = (int *)calloc(MAX_SV, sizeof(int));
    for (i=0; i<count; i++) 
	{
		char full_name[MAX_SIZE*2];
		sprintf(full_name,"%s %s",l_name + i*MAX_SIZE, f_name + i*MAX_SIZE);
        if ( strcasestr(full_name, name_find)) 
		{
			list_find[(*n)++] = i;
        }
    }
	return list_find;
}

int *find_class(char *name_find, int *n)
{
	int i;
	int *list_find = (int *)calloc(MAX_CLASS, sizeof(int));
    for (i=0; i<count_class; i++) 
	{
        if ( strcasestr(name_class+i*MAX_SIZE, name_find)) 
		{
			list_find[(*n)++] = i;
        }
    }
	return list_find;
}


void find_and_delete()
{
	clear();
	box(stdscr,0,0);
	y=1;
	strcpy(text,"\u300CXóa Sinh Viên\u300D");
	int col = (COLS - strlen(text))/2;
	mvprintw(y++,col,"%s",text);

	echo();
	curs_set(1);
	char name_find[MAX_SIZE];
	mvprintw(y,3,"%s","Nhập tên sinh viên: ");
    mvgetnstr(y,23,name_find,MAX_SIZE-1);
	noecho();
	curs_set(0);
	y++;

	int n = 0, *list_find;
	list_find = find(name_find, &n);

	if (n==0)
	{
		mvprintw(y++,3,"Không có sinh viên có tên là %s !!!",name_find);
		refresh();
		getch();
		free(list_find);
		return;
	}
	mvprintw(y++,3,"Có %d sinh viên có tên là %s gồm:",n,name_find);
	int i=0;
	
	while (TRUE)
	{
		int e;
		for (e=0;e<=6;e++)
		{
			move(y+e,0);
			clrtoeol();
		}

		if (n==0)
		{
			mvprintw(y-1,3,"Không còn sinh viên nào tên là %s !!!",name_find);
			refresh();
			getch();
			free(list_find);
			return;
		}

		mvprintw(y+1,6,"Sinh viên thứ %d",i+1);
		mvprintw(y+2,3,"Họ và Tên: %s %s",l_name+list_find[i]*MAX_SIZE, f_name+list_find[i]*MAX_SIZE);
		mvprintw(y+3,3,"Giới tính: %s",(sex[list_find[i]])?"Nam":"Nữ");
		if (id[list_find[i]]==-1) mvprintw(y+4,3,"ID: Chưa có");
		else mvprintw(y+4,3,"ID: %d",id[list_find[i]]);
		mvprintw(y+5,3,"Địa chỉ: %s",address+list_find[i]*MAX_SIZE);
		mvprintw(y+6,3,"Email: %s",((strcmp(email+list_find[i]*MAX_SIZE,"none")==0)?"Chưa có":email+list_find[i]*MAX_SIZE));

		strcpy(text,"delete");
		col = (COLS - strlen(text))/2;
		mvprintw(y+7,col,"%s",text);
		strcpy(text,"[Enter]");
		col = (COLS - strlen(text))/2;
		mvprintw(y+8,col,"%s",text);

		strcpy(text,"prev");
		col = COLS/6;
		mvprintw(y+7,col,"%s",text);
		strcpy(text,"[<-]");
		col = COLS/6 + (strlen("prev")-strlen("[<-]"))/2;
		mvprintw(y+8,col,"%s",text);

		strcpy(text,"next");
		col =(COLS-COLS/6) - (strlen("next")/2);
		mvprintw(y+7,col,"%s",text);
		strcpy(text,"[->]");
		col = (COLS-COLS/6) - (strlen("next")-strlen("[->]"))/2;
		mvprintw(y+8,col,"%s",text);

		strcpy(text,"exit");
		col = (COLS - strlen(text))/2;
		mvprintw(y+10,col,"%s",text);		
		strcpy(text,"[Esc]");
		col = (COLS - strlen(text))/2;
		mvprintw(y+11,col,"%s",text);	

		box(stdscr,0,0);
		refresh();
		event = getch();
		switch(event)
		{
			case KEY_LEFT:
			{
				i--;
				if (i<0) i=n-1;
				break;
			}
			case KEY_RIGHT:
			{
				i++;
				if (i>n-1) i=0;
				break;
			}
			case 10: //Nút Enter
			{
				int e;
				for (e=list_find[i]; e<count-1; e++)
				{
					strcpy(l_name+e*MAX_SIZE, l_name+(e+1)*MAX_SIZE);
					strcpy(f_name+e*MAX_SIZE, f_name+(e+1)*MAX_SIZE);
					strcpy(birthday+e*MAX_SIZE, birthday+(e+1)*MAX_SIZE);
					sex[e]= sex[e+1];
					strcpy(address+e*MAX_SIZE, address+(e+1)*MAX_SIZE);
					strcpy(email+e*MAX_SIZE, email+(e+1)*MAX_SIZE);
					id[e]= id[e+1];
				}
				count--;

				for (e=i; e<n-1;e++)
				{
					list_find[e] = list_find[e+1] - 1;// vì các cái đăng sau bị dịnh sang trái nên phải giảm chỉ số
				}
				n--;
				if (i==n) i--;

				for (e=0;e<=6;e++)
				{
					move(y+e,0);
					clrtoeol();
				}

				box(stdscr,0,0);
				strcpy(text,"Xóa thành công");
				int col = (COLS - strlen(text))/2;
				mvprintw(y+3,col,"%s",text);

				save(name_file);
				getch();
				break;
			}
			case 27: // Nút Esc
			{
				free(list_find);
				return;
			}
		}
	}
}

void assign_id(int id_faculty)
{
	int i;
	int time_stop = 150000; // dừng 0.15 s
	for (i=0;i<2;i++)
	{
		mvprintw(7,25," [.    ]");
		refresh();
		usleep(time_stop);
		mvprintw(7,25," [..   ]");
		refresh();
		usleep(time_stop);
		mvprintw(7,25," [...  ]");
		refresh();
		usleep(time_stop);
		mvprintw(7,25," [ ... ]");
		refresh();
		usleep(time_stop);
		mvprintw(7,25," [  ...]");
		refresh();
		usleep(time_stop);
		mvprintw(7,25," [   ..]");
		refresh();
		usleep(time_stop);
		mvprintw(7,25," [    .]");
		refresh();
		usleep(time_stop);
		mvprintw(7,25," [     ]");
		refresh();
		usleep(2*time_stop);
	}
	move(7,25);
	clrtoeol();

	// kiểm tra có được sắp xếp hay chx
	for (i=1; i<count; i++)
	{
		char full_name_i[MAX_SIZE*2];
		sprintf(full_name_i, "%s %s", l_name + i*MAX_SIZE, f_name + i*MAX_SIZE);

		char full_name_prev[MAX_SIZE*2];
		sprintf(full_name_prev, "%s %s", l_name + (i-1)*MAX_SIZE, f_name + (i-1)*MAX_SIZE);

		if (strcmp(full_name_prev,full_name_i) > 0)
		{
			strcpy(text,"(Danh sách chx đc sắp xếp)");
			box(stdscr,0,0);
			mvprintw(7,25,"%s",text);
			getch();
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
	int year_class=0;
	sscanf(name_file, "class/%2d", &year_class);
	for (i=0;i<count;i++)
	{
		if (id[i]==-1) id[i] = id_faculty*1000000 + year_class*10000 + (++id_max);
	}

	strcpy(text,"(Cấp mã thành công)");
	mvprintw(7,25,"%s",text);
	save(name_file);

	box(stdscr,0,0);
	refresh();
	getch();
}

void assign_email()
{
	y=2;
	clear();
	box(stdscr,0,0);
	strcpy(text,"\u300CCấp Email Sinh Viên\u300D");
	int col = (COLS-strlen(text))/2;
	mvprintw(y++,col,"%s",text);

	int i;
	int success = 0;
	for (i=0; i<count; i++)
	{
		if (id[i]!=-1)
		{
			sprintf(email+i*MAX_SIZE, "%d@sv1.dut.udn.vn",id[i]);
			success++;
		}
	}
	
	mvprintw(y++,3,"Đã hoàn thành cấp Email.");
	mvprintw(y++,3,"Cấp email thành công %d/%d",success,count);
	save(name_file);
	refresh();
	getch();
	return;
}

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

void sort_by_name()
{
	int i;
	int time_stop = 150000; // 0.15s
	for (i=0;i<2;i++)
	{
		mvprintw(4,26," [.    ]");
		refresh();
		usleep(time_stop);
		mvprintw(4,26," [..   ]");
		refresh();
		usleep(time_stop);
		mvprintw(4,26," [...  ]");
		refresh();
		usleep(time_stop);
		mvprintw(4,26," [ ... ]");
		refresh();
		usleep(time_stop);
		mvprintw(4,26," [  ...]");
		refresh();
		usleep(time_stop);
		mvprintw(4,26," [   ..]");
		refresh();
		usleep(time_stop);
		mvprintw(4,26," [    .]");
		refresh();
		usleep(time_stop);
		mvprintw(4,26," [     ]");
		refresh();
		usleep(2*time_stop);
	}
	move(4,26);
	clrtoeol();
	if (count>0)
	{
		sort_list(0,count-1);
		save(name_file);
		mvprintw(4,26," (Sắp xếp thành công)");
	}
	else
	{
		mvprintw(4,26," (Danh sách rỗng)");
	}
	box(stdscr,0,0);
	refresh();
	
	getch();
	move(4,26);
	clrtoeol();
	box(stdscr,0,0);
	refresh();
}

void find_students()
{
	clear();
	box(stdscr,0,0);
	y=1;
	strcpy(text,"\u300CTìm Sinh Viên\u300D");
	int col = (COLS - strlen(text))/2;
	mvprintw(y++,col,"%s",text);

	echo();
	curs_set(1);
	char name_find[MAX_SIZE];
	mvprintw(y,3,"%s","Nhập tên sinh viên: ");
    mvgetnstr(y,23,name_find,MAX_SIZE-1);
	noecho();
	curs_set(0);
	y++;

	int n = 0, *list_find;
	list_find = find(name_find, &n);

	if (n==0)
	{
		mvprintw(y++,3,"Không có sinh viên có tên là %s !!!",name_find);
		refresh();
		getch();
		free(list_find);
		return;
	}
	mvprintw(y++,3,"Sinh viên có tên là %s gồm:",name_find);
	int i=0;
	
	while (TRUE)
	{
		move(y,6);
		clrtoeol();
		mvprintw(y+1,6,"Sinh viên thứ %d",i+1);
		
		move(y+2,3);
		clrtoeol();
		mvprintw(y+2,3,"Họ và Tên: %s %s",l_name+list_find[i]*MAX_SIZE, f_name+list_find[i]*MAX_SIZE);
		
		move(y+3,3);
		clrtoeol();mvprintw(y+3,3,"Giới tính: %s",(sex[list_find[i]])?"Nam":"Nữ");
		if (id[list_find[i]]==-1)
		{
			move(y+4,3);
			clrtoeol();
			mvprintw(y+4,3,"ID: Chưa có");
		}
		else
		{
			move(y+4,3);
			clrtoeol();
			mvprintw(y+4,3,"ID: %d",id[list_find[i]]);
		}

		move(y+5,3);
		clrtoeol();
		mvprintw(y+5,3,"Địa chỉ: %s",address+list_find[i]*MAX_SIZE);
		
		move(y+6,3);
		clrtoeol();
		mvprintw(y+6,3,"Email: %s",((strcmp(email+list_find[i]*MAX_SIZE,"none")==0)?"Chưa có":email+list_find[i]*MAX_SIZE));

		// reset dòng prev delete next
		move(y+7,0);
		clrtoeol();
		move(y+8,0);
		clrtoeol();

		strcpy(text,"exit");
		col = (COLS - strlen(text))/2;
		mvprintw(y+7,col,"%s",text);
		strcpy(text,"[Esc]");
		col = (COLS - strlen(text))/2;
		mvprintw(y+8,col,"%s",text);

		if (i>0)
		{
			strcpy(text,"prev");
			col = COLS/6;
			mvprintw(y+7,col,"%s",text);
			strcpy(text,"[<-]");
			col = COLS/6 + (strlen("prev")-strlen("[<-]"))/2;
			mvprintw(y+8,col,"%s",text);
		}

		if (i<n-1)
		{
			strcpy(text,"next");
			col =(COLS-COLS/6) - (strlen("next")/2);
			mvprintw(y+7,col,"%s",text);
			strcpy(text,"[->]");
			col = (COLS-COLS/6) - (strlen("next")-strlen("[->]"))/2;
			mvprintw(y+8,col,"%s",text);
		}
		box(stdscr,0,0);

		refresh();
		event = getch();
		switch(event)
		{
			case KEY_LEFT:
			{
				if (i>0) i--;
				break;
			}
			case KEY_RIGHT:
			{
				if (i<n-1) i++;
				break;
			}
			case 27: // Nút Esc
			{
				free(list_find);
				return;
			}
		}
	}
}

void add_class()
{
	clear();
	box(stdscr,0,0);
	y = 1;

	strcpy(text,"\u300CADD CLASS\u300D");
	int col = (COLS - strlen(text))/2;
	mvprintw(y++,col,"%s",text);
	refresh();

	char inp[MAX_SIZE];
	is_valid_input = FALSE;
	int e;
	curs_set(1); // con trỏ đứng nháy nháy
	echo();      // hiện lên những gì nhập vào màn hình
	while (!is_valid_input)
	{
		is_valid_input = TRUE;
		mvprintw(y, 5, "Nhập số lượng lớp cần thêm: ");
		mvgetnstr(y, 34, inp, MAX_SIZE - 1);
		is_valid_input = check_int(inp,&e);
		if (!is_valid_input || e<0)
		{
			is_valid_input = FALSE;
			move(y,34);
			clrtoeol(); // xóa dữ liệu từ vị trí con trỏ đến hết dòng
			box(stdscr,0,0);
			mvprintw(y+1,5,"Vui lòng nhập số nguyên dương !!!");
		}

	}
	move(++y,5);
	clrtoeol();
	box(stdscr,0,0);
	
	int i=1;
	while (e--)
	{
		clear();
		y = 1;
		box(stdscr,0,0);

		mvprintw(y++,5,"Hãy nhập thông tin lớp thứ %d",i++);
		
		is_valid_input = FALSE;
		while (!is_valid_input)
		{
			is_valid_input = TRUE;
			mvprintw(y,5,"Mã lớp (Năm + lớp): ");
			mvgetnstr(y, 34, name_class+count_class*MAX_SIZE, MAX_SIZE - 1);

			if (check_name_class(name_class+count_class*MAX_SIZE)==FALSE)
			{
				is_valid_input = FALSE;
				move(y,34);
				clrtoeol();
				box(stdscr,0,0);
				mvprintw(y+1,5,"Hãy nhập lại (Năm + Lớp | VD:25KHDL) !!!");
				continue;
			}

			int j;
			for (j=0;j<count_class;j++)
			{
				if (strcmp(name_class+j*MAX_SIZE, name_class+count_class*MAX_SIZE) == 0)
				{
					is_valid_input = FALSE;
					move(y,34);
					clrtoeol();
					box(stdscr,0,0);
					mvprintw(y+1,5,"Lớp đã tồn tại !!!");
					break;
				}
			}
		}
		move(++y,5);
		clrtoeol();
		box(stdscr,0,0);
		
		is_valid_input = FALSE;
		while (!is_valid_input)
		{
			is_valid_input = TRUE;
			mvprintw(y,5,"Mã khoa: ");
			mvgetnstr(y, 34, inp, MAX_SIZE - 1);
			is_valid_input = check_int(inp,&faculty_class[count_class]);
			if (!is_valid_input)
			{
				move(y,34);
				clrtoeol();
				box(stdscr,0,0);
				mvprintw(y+1,5,"Không hợp lệ, vui lòng nhập lại (Số nguyên có 3 chữ số) !!!");
			}
		}
		move(++y,5);
		clrtoeol();
		box(stdscr,0,0);

		// tạo file/cập nhật tên vào name_class.txt
		char name_file[MAX_SIZE+20];
		sprintf(name_file,"class/%s.txt",name_class+count_class*MAX_SIZE);
		FILE *file_1 = fopen(name_file,"w");
		if (file_1!=NULL) fclose(file_1);

		FILE *file_2 = fopen("name_class.txt","a");
		if (file_2!=NULL)
		{
			fprintf(file_2,"%s:%d\n",name_class+count_class*MAX_SIZE,faculty_class[count_class]);
			fclose(file_2);
		}

		count_class++;
	}
	mvprintw(y++,5,"Hoàn thành bổ xung lớp!");

	noecho();    // tắt những gì đc nhập vào màn hình
	curs_set(0); // tắt con trỏ đứng

	getch();
}

void delete_class()
{
	clear();
	box(stdscr,0,0);
	y=1;
	strcpy(text,"\u300CDELETE CLASS\u300D");
	int col = (COLS - strlen(text))/2;
	mvprintw(y++,col,"%s",text);

	echo();
	curs_set(1);
	char name_find[MAX_SIZE];
	mvprintw(y,3,"%s","Nhập mã lớp: ");
    mvgetnstr(y,16,name_find,MAX_SIZE-1);
	noecho();
	curs_set(0);
	y++;

	int n = 0, *list_find;
	list_find = find_class(name_find, &n);

	if (n==0)
	{
		mvprintw(y++,3,"Không có tồn tại mã lớp %s !!!",name_find);
		refresh();
		getch();
		free(list_find);
		return;
	}
	mvprintw(y++,3,"Có %d lớp liên quan %s gồm:",n,name_find);
	int i=0;
	
	while (TRUE)
	{
		int e;
		for (e=-1;e<=2;e++)
		{
			move(y+e,0);
			clrtoeol();
		}
		
		mvprintw(y,6,"Lớp thứ %d",i+1);
		mvprintw(y+1,6,"Mã lớp: %s",name_class+list_find[i]*MAX_SIZE);
		mvprintw(y+2,6,"Mã khoa: %d",faculty_class[list_find[i]]);

		strcpy(text,"delete");
		col = (COLS - strlen(text))/2;
		mvprintw(y+4,col,"%s",text);
		strcpy(text,"[Enter]");
		col = (COLS - strlen(text))/2;
		mvprintw(y+5,col,"%s",text);

		strcpy(text,"prev");
		col = COLS/6;
		mvprintw(y+4,col,"%s",text);
		strcpy(text,"[<-]");
		col = COLS/6 + (strlen("prev")-strlen("[<-]"))/2;
		mvprintw(y+5,col,"%s",text);

		strcpy(text,"next");
		col =(COLS-COLS/6) - (strlen("next")/2);
		mvprintw(y+4,col,"%s",text);
		strcpy(text,"[->]");
		col = (COLS-COLS/6) - (strlen("next")-strlen("[->]"))/2;
		mvprintw(y+5,col,"%s",text);

		strcpy(text,"exit");
		col = (COLS - strlen(text))/2;
		mvprintw(y+7,col,"%s",text);		
		strcpy(text,"[Esc]");
		col = (COLS - strlen(text))/2;
		mvprintw(y+8,col,"%s",text);	

		box(stdscr,0,0);
		refresh();
		event = getch();
		switch(event)
		{
			case KEY_LEFT:
			{
				i--;
				if (i<0) i=n-1;
				break;
			}
			case KEY_RIGHT:
			{
				i++;
				if (i>n-1) i=0;
				break;
			}
			case 10: //Nút Enter
			{
				char name_del[MAX_SIZE+20];
				sprintf(name_del, "class/%s.txt", name_class+list_find[i]*MAX_SIZE);
				remove(name_del);
				count_class--;

				int e;
				for (e=list_find[i]; e<count_class; e++)
				{
					strcpy(name_class+e*MAX_SIZE, name_class+(e+1)*MAX_SIZE);
					faculty_class[e]= faculty_class[e+1];
				}

				for (e=-1;e<=2;e++)
				{
					move(y+e,0);
					clrtoeol();
				}

				for (e=i;e<n-1;e++)
				{
					list_find[e]=list_find[e+1]-1;
				}
				n--;
				if (i==n) i--;

				box(stdscr,0,0);
				strcpy(text,"Xóa thành công");
				int col = (COLS - strlen(text))/2;
				mvprintw(y,col,"%s",text);

				// Viết lại file tên lớp

				FILE *file = fopen("name_class.txt","w");
				if (file!=NULL)
				{
					for (e=0;e<count_class;e++)
					{
						fprintf(file,"%s:%d\n",name_class+e*MAX_SIZE,faculty_class[e]);
					}
					fclose(file);
				}

				getch();
				return;
			}
			case 27: // Nút Esc
			{
				free(list_find);
				return;
			}
		}
	}
}

void select_create_class()
{
	setlocale(LC_ALL, "");
    //Khoi tao ncurses
    initscr();
	set_escdelay(1);// vì getch() khi đọc esc tốn 1s, chỉnh lại xuống 1 mili

	if (has_colors()) {        // Nếu Terminal có hỗ trợ màu không
		start_color();
		init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Chữ , Nền
	}
    noecho();             // Tắt giao diện nhập
    curs_set(0);          // Ẩn con trỏ chuột
    keypad(stdscr, TRUE); // Cho phép sử dụng các phím đặc biệt (F1,F2,Enter,...)

	while (run)
	{
		clear();           // làm sạch màn hình
		box(stdscr, 0, 0); // vẽ khung
		switch(page)
		{
			// trang hub
			case 0:
			{
				y=0;
				// vẽ
				strcpy(text,"MENU");
				int col = (COLS - strlen(text))/2;
				mvprintw(1,col,"%s",text);
		
				while (y<n_page0)
				{
					if (y!=highlight)
					{
						mvprintw(2+y,3,"  %s  ",choices_page0[y]);
						y++;
					}
					else
					{
						attron(COLOR_PAIR(1));
						mvprintw(2+y,3,"\u2192 %s \u2190",choices_page0[y]);
						attroff(COLOR_PAIR(1));
						y++;
					}
				}
				
				// tương tác
				event = getch();
				switch(event)
				{
					case KEY_UP:
					{
						highlight = (highlight>0)?highlight-1:n_page0-1;
						break;
					}
					
					case KEY_DOWN:
					{
						highlight = (highlight<n_page0-1)?highlight+1:0;
						break;
					}

					case 10://nút Enter
					{
						select_choice = highlight;
						break;
					}
				}

				switch(select_choice)
				{
					case 0:
					{
						page=1;// chuyển tới trang chọn lớp
						// thiết lập lại "chọn"
						highlight = 0;
						select_choice = -1;
						break;
					}
					case 1:
					{
						page=2;// chuyển tới trang thêm lớp
						// thiết lập lại "chọn"
						highlight = 0;
						select_choice = -1;
						break;
					}
					case 2:
					{
						run = FALSE;
						break;
					}
				}
				break;
			}

			// trang chọn lớp
			case 1:
			{
				y=1;
				// vẽ
				strcpy(text,"SELECT CLASS");
				int col = (COLS - strlen(text))/2;
				mvprintw(y++,col,"%s",text);

				char *icon;
				if (print==TRUE) icon= "\u25BC";
				else icon = "\u25B6";
				if (highlight!=0 || print==TRUE) mvprintw(y++,3,"%s %s   ",icon,choices_page1[0]);
				else
				{
					attron(COLOR_PAIR(1));
					mvprintw(y++,3,"%s %s \u2190",icon,choices_page1[0]);
					attroff(COLOR_PAIR(1));
				}

				if (print==TRUE)
				{
					if (count_class==0)
					{
						mvprintw(y++,3,"\u2514 Hiện không có lớp nào cả !!!");
					}
					else
					{
						char *prefix="\u251c";
						y=0;
						while (y<count_class)
						{
							if (y!=highlight)
							{
								mvprintw(3+y,7,"%s %s  ",prefix,name_class+y*MAX_SIZE);
								y++;
							}
							else
							{
								attron(COLOR_PAIR(1));
								mvprintw(3+y,7,"%s %s \u2190",prefix,name_class+y*MAX_SIZE);
								attroff(COLOR_PAIR(1));
								y++;
							}
						}
						prefix="\u2514";
						if (y!=highlight)
						{
							mvprintw(3+y,7,"%s %s  ",prefix,"Thu gọn");
							y++;
						}
						else
						{
							attron(COLOR_PAIR(1));
							mvprintw(3+y,7,"%s %s \u2190",prefix,"Thu gọn");
							attroff(COLOR_PAIR(1));
							y++;
						}
					}
				}

				if (highlight!=1 || print==TRUE) mvprintw(3+((print==TRUE)?count_class+1:0),5,"%s   ",choices_page1[1]);
				else
				{
					attron(COLOR_PAIR(1));
					mvprintw(3+((print==TRUE)?count_class+1:0),5,"%s \u2190",choices_page1[1]);
					attroff(COLOR_PAIR(1));
				}

				event = getch();
				if (print==FALSE)
				{
					switch(event)
					{
						case KEY_UP:
						{
							highlight = (highlight>0)?highlight-1:n_page1-1;
							break;
						}

						case KEY_DOWN:
						{
							highlight = (highlight<n_page1-1)?highlight+1:0;
							break;
						}

						case 10:// nút Enter
						{
							select_choice = highlight;
							break;
						}
					}

					switch(select_choice)
					{
						case 0:// hiện danh sách lớp
						{
							print = TRUE;
							// thiết lập lại "chọn"
							select_choice = -1;
							highlight = 0;
							break;
						}
						case 1: // quay lại trang hub
						{
							page = 0;
							// thiết lập lại "chọn"
							select_choice = -1;
							highlight = 0;
							break;
						}
					}
				}
				else
				{
					switch(event)
					{
						case KEY_UP:
						{
							highlight = (highlight>0)?highlight-1:count_class;
							break;
						}

						case KEY_DOWN:
						{
							highlight = (highlight<count_class)?highlight+1:0;
							break;
						}

						case 10:// nút Enter
						{
							if (highlight==count_class)
							{
								print = FALSE;
								highlight = 0;
								select_choice = -1;
								break;
							}
							page = 3;
							// lấy dữ liệu lớp
							current_class = highlight;
							sprintf(name_file,"class/%s.txt",name_class+(current_class)*MAX_SIZE);
							is_data_loaded = get_data(name_file);
							// thiết lập lại "chọn"
							select_choice = -1;
							highlight = 0;
							break;
						}
					}
				}
				refresh();
				break;
			}

			// trang tương tác lớp
			case 3:
			{
				menu(faculty_class[current_class],is_data_loaded);
				break;
			}

			// trang thêm lớp
			case 2:
			{
				y=1;
				// vẽ
				sprintf(text,"%s","EDIT CLASS");
				int col = (COLS - strlen(text))/2;
				mvprintw(y++,col,"%s",text);
				refresh();

				int i;
				for (i=0;i<n_page2;i++)
				{
					if (i==highlight)
					{
						attron(COLOR_PAIR(1));
						mvprintw(y+i,3,"%s \u2190",choices_page2[i]);
						attroff(COLOR_PAIR(1));
					}
					else mvprintw(y+i,3,"%s  ",choices_page2[i]);
				}

				refresh();
				event = getch();
				switch(event)
				{
					case KEY_UP:
					{
						highlight--;
						if (highlight<0) highlight=n_page2-1;
						break;
					}

					case KEY_DOWN:
					{
						highlight++;
						if (highlight==n_page2) highlight=0;
						break;
					}

					case 10:// Nút Enter
					{
						select_choice = highlight;
						break;
					}
				}

				switch(select_choice)
				{
					case 0:
					{
						add_class();
						select_choice = -1;
						break;
					}

					case 1:
					{
						delete_class();
						select_choice = -1;
						break;
					}

					case 2:
					{
						select_choice = -1;
						highlight = 0;
						page = 0;
						break;
					}
				}

				break;
			}
		}
	}

	endwin();
}

void menu(int id_faculty, int check)
{
	if (check==FALSE)
	{
		strcpy(text,"Không tìm thấy file dữ liệu!!!");
		int col = (COLS-strlen(text))/2;
		mvprintw(3,col,"%s",text);
		getch();
		page = 1;
		
		return;
	}


	strcpy(text,name_file);
	text[strlen(text)-4]='\0';
	int col = (COLS-strlen(text+6))/2;
	mvprintw(1,col,"%s",text+6);

	char *prefix;
	char *select   = "\u2500";
	char *select_l = "\u300C";
	char *select_r = "\u300D";

	y=0;

	mvprintw(2,1,"[%s]","MENU");

	while (y<n_menu)
	{
		if (y<n_menu-1) prefix="\u251c";
		else prefix="\u2514";
		if (y!=highlight)
		{
			mvprintw(3+y,3,"%s %s ",prefix,choices_menu[y]);
			y++;
		}
		else
		{
			attron(COLOR_PAIR(1));
			mvprintw(3+y,3,"%s%s%s%s%s",prefix,select,select_l,choices_menu[y],select_r);
			attroff(COLOR_PAIR(1));
			y++;
		}
	}

	event = getch();
	switch(event)
	{
		case KEY_UP:
		{
			highlight = (highlight>0)?highlight-1:n_menu-1;
			break;
		}
		
		case KEY_DOWN:
		{
			highlight = (highlight<n_menu-1)?highlight+1:0;
			break;
		}

		case 10:// Nút Enter
		{
			select_choice = highlight;
			break;
		}
	}

	switch(select_choice)
	{
		case 0: // Thêm sinh viên
		{
			select_choice = -1;
			add();
			save(name_file);
			break;
		}

		case 1: // Sắp xếp danh sách
		{
			select_choice = -1;
			sort_by_name();
			break;
		}

		case 2: // Xóa sinh viên
		{
			select_choice = -1;
			find_and_delete();
			break;
		}

		case 3: // Tim sinh viên
		{
			select_choice = -1;
			find_students();
			break;
		}

		case 4: // Cấp mã sinh viên
		{
			assign_id(faculty_class[current_class]);
			select_choice = -1;
			break;
		}

		case 5: // Cấp Email sinh viên
		{
			assign_email();
			select_choice = -1;
			break;
		}

		case 6: // In ra danh sách sinh viên
		{
			show();
			select_choice = -1;
			break;
		}

		case 7: // Nút thoát
		{
			page = 1;
			select_choice = -1;
			highlight = 0;
			break;
		}
	}
	refresh();
}
