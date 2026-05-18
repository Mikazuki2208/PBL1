# 🎓 PBL1: Ứng dụng Quản lý Danh sách Sinh viên

Dự án xây dựng phần mềm quản lý sinh viên bằng ngôn ngữ C. Mã nguồn được chia thành hai phiên bản (Bản chính và Bản phụ) để tiện cho việc theo dõi và phát triển. 

> **Lưu ý:** Bạn có thể tải toàn bộ các tệp của cả hai phiên bản về cùng một thư mục chung. Chúng hoạt động độc lập và không hề gây xung đột với nhau.

---

## 🚀 Hướng dẫn Cài đặt và Chạy chương trình

Dưới đây là hướng dẫn chi tiết để biên dịch và chạy ứng dụng trên môi trường Terminal.

### 🌟 Bản chính (Khuyên dùng)
Đây là phiên bản hoàn thiện, mã nguồn được chia mô-đun rõ ràng và có sử dụng thư viện `ncurses` cho giao diện terminal.

* **Các tệp yêu cầu:** `main.c`, `manager.c`, `manager.h`, `name_class.txt`, `class`

**Các bước thực hiện:**

1. **Khởi tạo môi trường ảo (Chỉ dành cho lần chạy đầu tiên):**
   Cài đặt môi trường chứa trình biên dịch `gcc` và thư viện `ncurses`.
   ```bash
   nix-shell -p gcc ncurses
