#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>

// ============================================================
// Book 类 —— 对"图书"的抽象与封装
// ------------------------------------------------------------
// 私有数据成员描述一本书的基本信息（书名、ISBN、出版社、价格、
// 页数、馆藏数量）；公有成员函数对外提供初始化、修改、查询、
// 输出等统一接口。外部代码只能通过接口访问数据，
// 从而把"合法性校验"集中在类内部统一完成（见各 setter）。
// ============================================================
class Book {
private:
    std::string name;       // 图书名称
    std::string isbn;       // 图书 ISBN（一本书的唯一标识，借还时据此匹配）
    std::string publisher;  // 出版社信息
    double      price;      // 价格（单位：元，合法范围 >= 0）
    int         pages;      // 页数（合法范围 > 0）
    int         stock;      // 馆藏数量（在馆可借的册数，合法范围 >= 0）

public:
    // ===== 构造函数（对象初始化）=====
    Book();   // 默认构造：一本信息为空的书，库存默认 1 册

    // 带参构造：一次性给出图书全部信息，stock 缺省为 1
    // 构造函数体内会对每个成员做合法性校验，非法值会被纠正并打印警告
    Book(const std::string& name,
         const std::string& isbn,
         const std::string& publisher,
         double             price,
         int                pages,
         int                stock = 1);

    // ===== 修改操作（setter，均带合法性校验，非法输入将被拒绝）=====
    void setName(const std::string& name);      // 名称不能为空
    void setISBN(const std::string& isbn);      // 必须通过 ISBN 校验
    void setPublisher(const std::string& publisher); // 出版社不能为空
    void setPrice(double price);                // 价格不能为负
    void setPages(int pages);                   // 页数必须 > 0
    void setStock(int stock);                   // 库存不能为负

    // 一次性设置所有数据成员。
    // 与逐个调用 setter 的区别：某个字段非法时只跳过该字段，
    // 其余合法字段照常更新，不会因一个字段非法而全部失败。
    void setAll(const std::string& name,
                const std::string& isbn,
                const std::string& publisher,
                double             price,
                int                pages,
                int                stock = 1);

    // ===== 获取操作（getter）=====
    // 末尾的 const 承诺：该函数只读数据、不会修改对象状态，
    // 因此 const 对象也可以调用这些函数。
    std::string getName()      const;
    std::string getISBN()      const;
    std::string getPublisher() const;
    double      getPrice()     const;
    int         getPages()     const;
    int         getStock()     const;

    // 派生状态（不单独存储，由 stock 计算得到）：
    // 库存数量 > 0 即"可借"，否则为"已借完"
    bool isAvailable() const;

    // ===== 输出操作 =====
    // 打印图书完整信息。参数 os 默认指向标准输出 std::cout，
    // 也可传入文件流等其他 ostream，实现同一份代码输出到不同目标。
    void display(std::ostream& os = std::cout) const;

    // ===== 其他操作：ISBN 合法性校验（静态成员函数）=====
    // static 表示它从属于"类"而非某个对象，无需创建 Book 对象即可调用：
    //     bool ok = Book::validateISBN("978-7-115-27946-0");
    // 支持 ISBN-10 和 ISBN-13，输入中允许包含连字符或空格。
    // 返回 true 表示校验通过，false 表示不合法。
    static bool validateISBN(const std::string& isbn);
};

#endif // BOOK_H
