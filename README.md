# PBL1
Build a student list management application

có thể tải hết về 1 thư mục, chúng không ảnh hưởng tới nhau

# Bản phụ
file tải về

  { code_old.c | name_class.txt | class }

terminal

  "gcc code_old.c -o main" (chuyển code thành app)
  
  "./main"                 (chạy app)

# Bản chính
file tải về

  { main.c | manager.c | manager.h | name_class.txt | class }

terminal

  "nix-shell -p gcc ncurses"                 (Dùng lần đầu tiên: để tạo môi trường ảo)
  
  "gcc main.c manager.c -o main -lncursesw"  (Chuyển code thành app)
  
  ./main"                                    (Chạy app)
