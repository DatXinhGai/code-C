//
// Created by HP LAPTOP on 02/01/2025.
//

// cú pháp

enum EnumName {
    CONST1,         // Mặc định là 0
    CONST2,         // 1
    CONST3 = 5,     // Gán giá trị 5
    CONST4,         // 6 (liên tiếp)
};
// tên các hằng số phải duy nhất trong cùng 1 enum
// các hằng số được ánh xạ thành số nguyên nên có thể dùng trong biểu thức số học
// thay vì phải sử dụng số thì có thể dùng tên dễ hiểu như RED, YELLOW, GREEN
// dễ bảo trì do chỉ cần sửa trong enum, không cần sửa nhiều chỗ trong code
// thường dùng biểu diễn status, quyền, hoặc ngày, tháng...
