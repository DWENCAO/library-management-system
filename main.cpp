// ============================================================
// main.cpp —— 图书馆借阅系统演示程序（程序入口）
// ------------------------------------------------------------
// 演示流程分四部分：
//   1. 创建 6 本图书馆藏（Book 对象，设置不同库存）；
//   2. 调用静态函数 Book::validateISBN，分别用合法/非法 ISBN
//      验证 ISBN 校验功能；
//   3. 创建 2 名学生（Student 对象），按预设场景依次借书，
//      覆盖"成功 / 重复借 / 无库存 / 超上限"等全部判断分支；
//   4. 还书并再次借书，验证双方状态能正确恢复与联动。
//
// 另：用 std::vector<Book*> 保存所有图书的地址，并通过独立
//     全局函数按书名检索图书。
//
// 语法提示：调用 stu.borrowBook(&cppPrimer) 时，& 是"取地址符"，
// 把图书馆里真实图书的地址传给形参 Book*，这样函数内部对库存
// 的修改才会作用到这里的原对象上（依赖关系）。
// ============================================================

#include "Book.h"
#include "Student.h"
#include <iostream>
#include <vector>
#include <string>

// ============================================================
// 独立全局函数：按图书名称检索图书并输出匹配结果
// ------------------------------------------------------------
// 参数：
//   books   —— 保存图书馆中所有图书对象地址的 vector 容器；
//   keyword —— 要检索的书名关键词（支持"包含匹配"，即书名中
//              含有该关键词即视为命中，区分大小写）。
// 实现：遍历 vector 中的每个 Book* 指针，取出书名进行比较，
//       命中则输出该书完整信息，最后统计并输出匹配数量。
// 说明：容器中存放的是图书地址（Book*），不会产生 Book 对象
//       的副本，检索到的就是图书馆中的原书。
// ============================================================
void searchBookByName(const std::vector<Book*>& books,
                      const std::string&        keyword) {
    std::cout << "\n----- 按书名检索：\"" << keyword << "\" -----\n";

    int matchCount = 0;   // 命中的图书数量

    // 范围 for 遍历容器；book 是 Book* 类型，用 -> 访问其成员函数
    for (Book* book : books) {
        // 空指针保护：跳过容器中可能存在的空地址
        if (book == nullptr) {
            continue;
        }

        // find 返回关键词在书名中的位置；返回 npos 表示"未找到"
        if (book->getName().find(keyword) != std::string::npos) {
            std::cout << "  [命中] 找到一本匹配图书：\n";
            book->display();      // 输出该书的完整信息
            ++matchCount;
        }
    }

    // 输出本次检索的汇总结果
    if (matchCount == 0) {
        std::cout << "  未找到书名包含 \"" << keyword << "\" 的图书。\n";
    } else {
        std::cout << "  检索完成，共找到 " << matchCount << " 本匹配图书。\n";
    }
    std::cout << "----------------------------------------\n";
}

int main() {
    // ===================== 1. 生成图书对象 =====================
    // 每本书设置不同的馆藏数量（stock），用于测试库存状态
    Book cppPrimer("C++ Primer Plus(第6版)",
                   "978-7-115-27946-0",
                   "人民邮电出版社", 99.00, 936, 3);   // 馆藏 3 册

    Book deepLearning("深度学习",
                      "978-7-115-46147-6",            // 修正后的合法 ISBN
                      "人民邮电出版社", 168.00, 788, 2); // 馆藏 2 册

    Book cPrimer("C Primer Plus(第6版)",
                 "978-7-115-52163-7",
                 "人民邮电出版社", 108.00, 541, 1);     // 馆藏 1 册

    Book algo("算法导论",
              "978-7-111-40701-0",
              "机械工业出版社", 128.00, 780, 1);         // 馆藏 1 册

    Book os("操作系统概念",
            "978-7-111-54493-7",
            "机械工业出版社", 99.00, 800, 1);            // 馆藏 1 册

    Book network("计算机网络：自顶向下方法",
                 "978-7-111-59273-0",
                 "机械工业出版社", 89.00, 700, 1);       // 馆藏 1 册

    // ===================== 1.5 将已实例化图书的地址存入 vector 容器 =====================
    // 容器元素类型为 Book*，用 & 取出每本图书对象的地址后压入容器。
    // 注意：这些图书对象在 main 结束前始终有效，因此容器中保存的
    //       地址在整个程序运行期间都是安全可用的。
    std::vector<Book*> bookList;
    bookList.push_back(&cppPrimer);
    bookList.push_back(&deepLearning);
    bookList.push_back(&cPrimer);
    bookList.push_back(&algo);
    bookList.push_back(&os);
    bookList.push_back(&network);

    // ===================== 1.6 书名检索功能测试（两处调用）=====================
    std::cout << "========== 图书检索功能测试 ==========\n";
    searchBookByName(bookList, "深度学习");   // 测试1：搜索一本"存在"的书
    searchBookByName(bookList, "数据结构");   // 测试2：搜索一本"不存在"的书

    // ===================== 2. 演示 validateISBN（让该功能发挥作用）=====================
    std::cout << "========== ISBN 合法性验证演示 ==========\n";

    const char* validISBNs[] = {
        "978-7-115-27946-0",   // ISBN-13 合法
        "9787115521637",       // ISBN-13 无分隔符 合法
        "978-7-111-40701-0",   // ISBN-13 合法
        "0-306-40615-2",       // ISBN-10 合法（经典示例）
    };
    const char* invalidISBNs[] = {
        "978-7-115-46147-7",   // ISBN-13 校验位错
        "1234567890123",       // ISBN-13 校验位错
        "7-115-15587-X",       // ISBN-10 校验位错
        "1234567890",          // ISBN-10 校验位错
        "abcdefghij",          // 非法字符
        "12345",               // 长度不对
    };

    std::cout << "\n--- 合法 ISBN ---\n";
    // 范围 for：依次取出数组中的每个 C 风格字符串指针
    for (const char* s : validISBNs) {
        bool ok = Book::validateISBN(s);   // 静态函数，通过"类名::函数名"直接调用
        std::cout << "  \"" << s << "\" -> "
                  << (ok ? "合法 [OK]" : "非法 [X]") << "\n";
    }

    std::cout << "\n--- 非法 ISBN ---\n";
    for (const char* s : invalidISBNs) {
        bool ok = Book::validateISBN(s);
        std::cout << "  \"" << s << "\" -> "
                  << (ok ? "合法 [OK]" : "非法 [X]") << "\n";
    }

    // ===================== 3. 生成学生对象 =====================
    Student stu("20230001", "张三", "计算机科学与技术");
    Student stu2("20230002", "李四", "软件工程");

    std::cout << "\n========== 初始状态 ==========\n\n";
    stu.display();
    std::cout << "\n各图书库存：\n";
    cppPrimer.display();
    deepLearning.display();
    cPrimer.display();
    algo.display();
    os.display();
    network.display();

    std::cout << "\n========== 开始借阅流程 ==========\n\n";
    // 下面的场景设计一一对应 borrowBook 内的判断分支：
    //   场景1 成功路径；场景2 命中"重复借阅"；场景3 正常借多本；
    //   场景4 由另一名学生命中"库存为 0"；场景4补充再次验证重复借阅；
    //   场景5 借满 5 本；场景6 命中"借阅上限"。

    // ---- 场景1：正常借书成功（库存充足）----
    stu.borrowBook(&cppPrimer);       // 成功，库存 3 -> 2

    // ---- 场景2：重复借阅同一本书（失败：已借过）----
    stu.borrowBook(&cppPrimer);       // 失败

    // ---- 场景3：继续借其他书 ----
    stu.borrowBook(&deepLearning);    // 成功，库存 2 -> 1
    stu.borrowBook(&cPrimer);         // 成功，库存 1 -> 0

    // ---- 场景4：另一学生借库存为 0 的书（失败：无库存）----
    stu2.borrowBook(&cPrimer);        // 失败

    // ---- 场景4补充：重复借阅同一本书（失败：已借过）----
    stu.borrowBook(&cPrimer);         // 失败

    // ---- 场景5：借满 5 本 ----
    stu.borrowBook(&algo);            // 成功
    stu.borrowBook(&os);              // 成功，已借 5 本，达上限

    // ---- 场景6：超过借阅上限（失败：达上限）----
    stu.borrowBook(&network);         // 失败

    std::cout << "\n========== 借满后查看状态 ==========\n\n";
    stu.displayBorrowedBooks();
    std::cout << "\n图书《" << cPrimer.getName() << "》当前库存: "
              << cPrimer.getStock() << " 册\n";

    // ===================== 4. 还书操作 =====================
    std::cout << "\n========== 还书流程 ==========\n\n";

    stu.returnBook(&cPrimer);         // 成功，库存 0 -> 1
    stu.borrowBook(&network);         // 成功，此时已借 5 本

    std::cout << "\n========== 最终状态 ==========\n\n";
    stu.display();
    stu.displayBorrowedBooks();

    std::cout << "\n各图书最终库存：\n";
    std::cout << "  《" << cppPrimer.getName()   << "》: " << cppPrimer.getStock()   << " 册\n";
    std::cout << "  《" << deepLearning.getName() << "》: " << deepLearning.getStock() << " 册\n";
    std::cout << "  《" << cPrimer.getName()      << "》: " << cPrimer.getStock()      << " 册\n";
    std::cout << "  《" << algo.getName()         << "》: " << algo.getStock()         << " 册\n";
    std::cout << "  《" << os.getName()           << "》: " << os.getStock()           << " 册\n";
    std::cout << "  《" << network.getName()      << "》: " << network.getStock()      << " 册\n";

    return 0;
}
