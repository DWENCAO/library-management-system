#ifndef STUDENT_H
#define STUDENT_H

#include "Book.h"   // 组合关系：Student 中嵌套 Book 类的对象数组
#include <string>
#include <iostream>

// ============================================================
// Student 类 —— 对"学生（借阅者）"的抽象与封装
// ------------------------------------------------------------
// 本类与 Book 类之间体现两种面向对象关系：
//
// 【组合关系（has-a，整体-部分）】
//   Student 内部内嵌一个 Book 对象数组 borrowedBooks，
//   表示该学生当前所借的图书记录。数组空间是学生对象的一部分，
//   学生对象销毁时这些借阅记录也随之消失。
//   约定：数组中下标 [0, borrowedCount) 的元素是有效记录。
//
// 【依赖关系（use-a，临时使用）】
//   borrowBook / returnBook 的形参是 Book*（图书指针），
//   指向 main 中图书馆侧的图书对象。借阅成功时必须同时修改
//   "书的库存"，所以要传地址（指针）而不是传值——
//   传值只会改动副本，图书馆里的真实库存不会变化。
// ============================================================
class Student {
private:
    std::string id;     // 学号
    std::string name;   // 姓名
    std::string major;  // 专业

    static const int MAX_BORROW = 5;   // 每人最多可借 5 本（类常量，所有学生共享）
    Book borrowedBooks[MAX_BORROW];    // 已借图书数组（组合：内嵌 Book 对象）
    int  borrowedCount;                // 当前已借数量（也是下一本可放入的下标）

public:
    // ===== 构造函数（初始化）=====
    Student();   // 默认构造：信息为空，借阅数量为 0
    Student(const std::string& id,
            const std::string& name,
            const std::string& major);

    // ===== 修改操作（setter）=====
    void setId(const std::string& id);
    void setName(const std::string& name);
    void setMajor(const std::string& major);

    // ===== 获取操作（getter，const 表示只读不修改）=====
    std::string getId()   const;
    std::string getName() const;
    std::string getMajor() const;
    int  getBorrowedCount() const;   // 当前已借数量
    int  getMaxBorrow()    const;    // 借阅上限

    // ===== 借阅功能（依赖关系：传 Book 指针，需修改书的库存）=====
    // 借书：依次判断"学生状态"（空指针/是否达上限/是否重复借阅）
    //       与"书的状态"（是否有库存），两者都允许才借阅成功。
    // 参数：book —— 图书馆中真实图书对象的地址（用 &书对象 传入）
    // 返回：true = 借书成功；false = 被某条规则拒绝
    bool borrowBook(Book* book);

    // 还书：从已借数组中移除该书，并通过指针把图书馆藏 +1。
    // 返回：true = 还书成功；false = 空指针或本就没借过这本书
    bool returnBook(Book* book);

    // 查询该学生是否已借过指定 ISBN 的书（防止重复借阅）
    bool hasBorrowed(const std::string& isbn) const;

    // ===== 输出操作（os 默认输出到屏幕，也可重定向到文件）=====
    void display(std::ostream& os = std::cout) const;              // 学生基本信息
    void displayBorrowedBooks(std::ostream& os = std::cout) const; // 当前借阅清单
};

#endif // STUDENT_H
