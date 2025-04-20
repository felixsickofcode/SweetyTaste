# 🎮 2D Platformer Game - Tiled Map Adventure

## 🧭 Giới thiệu

Đây là một trò chơi **platformer 2D** với bản đồ được thiết kế thủ công bằng **Tiled**. Người chơi điều khiển nhân vật chính khám phá các bản đồ, vượt chướng ngại vật và sống sót trước những thử thách. Trò chơi nhấn mạnh vào **yếu tố khám phá**, **điều khiển mượt mà** và **giao diện thân thiện**.

## 🎮 Điều khiển

| Phím | Chức năng         |
|------|-------------------|
| A    | Di chuyển trái    |
| D    | Di chuyển phải    |
| J    | Tấn công          |
| ESC  | Pause/Resume game |

## 🧠 Luật chơi

- **Game over** khi:
  - Nhân vật **hết máu**.
  - **Rơi khỏi bản đồ** (vực thẳm).
- Mục tiêu là **vượt qua bản đồ** và **khám phá toàn bộ khu vực**, không nhất thiết phải tiêu diệt tất cả kẻ địch.

## 🛠 Công cụ và thư viện sử dụng

- [Tiled](https://www.mapeditor.org/): Thiết kế bản đồ.
- [Tileson](https://github.com/SSBMTonberry/tileson): Đọc bản đồ `.json` từ Tiled.
- **SDL2**: Hiển thị đồ họa, âm thanh và quản lý sự kiện.

## 🔍 Các tính năng nổi bật

- 🎨 Vẽ hình bằng SDL2
- 🎞 Animation mượt mà (chạy, đánh, đứng yên, trúng đòn)
- 🌄 Background cuộn theo bản đồ
- 🎹 Bắt sự kiện bàn phím & chuột
- ⏱ Xử lý FPS ổn định
- 🧱 Va chạm AABB và theo tilemap
- 🩸 Thanh máu hiển thị rõ ràng
- 🧾 Tính điểm và lưu bảng điểm
- 🔊 Âm thanh hành động & nhạc nền
- 📝 Menu chính, Pause/Resume
- 🗺 Map được kiểm soát bởi tác giả

## 📚 Nguồn tham khảo

- [Phát Triển Phần Mềm 123A-Z (YouTube)](https://www.youtube.com/playlist?list=PLR7NDiX0QsfTIEQUeYCfc2MyCquX0ig9V)
- GPT hỗ trợ xử lý logic & fix bug 

---
