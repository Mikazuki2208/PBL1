# 🎓 PBL1: Ứng dụng Quản lý Danh sách Sinh viên

Dự án xây dựng phần mềm quản lý sinh viên bằng ngôn ngữ C. Mã nguồn bao gồm hai phiên bản (Bản chính và Bản phụ) được thiết kế độc lập, có thể tải về chung một thư mục mà không gây ảnh hưởng hay xung đột lẫn nhau.

---

## 🚀 Hướng dẫn Cài đặt và Chạy ứng dụng

Dưới đây là các câu lệnh thực thi chi tiết trong Terminal cho từng phiên bản.

### 🌟 1. Bản chính (Khuyên dùng)
Phiên bản hoàn thiện, mã nguồn được chia theo mô-đun rõ ràng và tích hợp thư viện `ncurses` hỗ trợ giao diện Terminal trực quan.

* **Các tệp yêu cầu:** `main.c`, `manager.c`, `manager.h`, `name_class.txt`, `class`
* **Các bước thực hiện:**

```bash
# Bước 1: Khởi tạo môi trường ảo (Chỉ cần chạy ở lần đầu tiên sử dụng)
nix-shell -p gcc ncurses

# Bước 2: Biên dịch các tệp mã nguồn thành ứng dụng
gcc main.c manager.c -o main -lncursesw

# Bước 3: Khởi chạy ứng dụng
./main
```

### 🌟 2. Bản phụ

* **Các tệp yêu cầu:** `code_old.c`, `name_class.txt`, `class`
* **Các bước thực hiện:**

```bash
# Bước 2: Biên dịch các tệp mã nguồn thành ứng dụng
gcc main.c -o main

# Bước 3: Khởi chạy ứng dụng
./main
```
