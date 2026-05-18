# PBL1
Build a student list management application

có thể tải hết về 1 thư mục, chúng không ảnh hưởng tới nhau

# Bản phụ
file tải về
  code_old.c | name_class.txt | class

terminal
  "gcc code_old.c -o main" | "./main"

# Bản chính
file tải về
  main.c | manager.c | manager.h | name_class.txt | class

terminal
  "nix-shell -p gcc ncurses"  | "gcc main.c manager.c -o main -lncursesw" | "./main"
