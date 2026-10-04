// ============================================================
// Book.cpp —— Book 类（图书）的成员函数实现
// ------------------------------------------------------------
// 本文件只包含函数实现，类的声明见 Book.h。
// 合法性校验的两种处理策略：
//   1) 构造函数：对象诞生时尚无"旧值"可保留，因此对非法值
//      采取"纠正为安全默认值"的策略，保证对象一出生就合法；
//   2) setter    ：对象已存在且可能正在被使用，因此对非法值
//      采取"拒绝修改、保留原值"的策略，避免破坏已有状态。
// ============================================================

#include "Book.h"
#include <iomanip>   // 输出格式控制：std::fixed、std::setprecision、std::left
#include <cctype>

// ================= 构造函数 =================

// 默认构造函数
// 冒号后面是"初始化列表"：在对象内存创建的同时直接初始化各成员，
// 比先构造再赋值效率更高，是 C++ 推荐的成员初始化方式。
Book::Book()
    : name(""), isbn(""), publisher(""), price(0.0), pages(0), stock(1)
{
}

// 带参构造函数：参数与私有成员同名时，用 this->成员 区分二者
Book::Book(const std::string& name,
           const std::string& isbn,
           const std::string& publisher,
           double             price,
           int                pages,
           int                stock)
    : name(name), isbn(isbn), publisher(publisher),
      price(price), pages(pages), stock(stock)
{
    // ===== 构造时即进行合法性验证（非法值纠正为安全默认值）=====
    if (name.empty()) {
        std::cerr << "[警告] 图书构造: 名称不能为空，已置空。\n";
        this->name = "";
    }
    // ISBN 交给静态校验函数统一判定，保证校验规则只有一处实现
    if (!validateISBN(isbn)) {
        std::cerr << "[警告] 图书构造: ISBN \"" << isbn
                  << "\" 不合法，已置空。\n";
        this->isbn = "";
    }
    if (price < 0.0) {
        std::cerr << "[警告] 图书构造: 价格不能为负，已置为 0。\n";
        this->price = 0.0;
    }
    if (pages <= 0) {
        std::cerr << "[警告] 图书构造: 页数必须大于 0，已置为 1。\n";
        this->pages = 1;
    }
    if (stock < 0) {
        std::cerr << "[警告] 图书构造: 库存数量不能为负，已置为 0。\n";
        this->stock = 0;
    }
}

// ================= 修改操作（setter 带合法性验证） =================
// 统一模式：先判断入参是否合法 —— 不合法则打印警告并 return
// （保留成员原值不动）；合法才执行赋值。警告统一输出到 std::cerr
// （标准错误流），与正常业务输出 std::cout 分开。

void Book::setName(const std::string& name) {
    if (name.empty()) {
        std::cerr << "[警告] setName: 名称不能为空，未修改。\n";
        return;
    }
    this->name = name;
}

void Book::setISBN(const std::string& isbn) {
    if (!validateISBN(isbn)) {
        std::cerr << "[警告] setISBN: ISBN \"" << isbn
                  << "\" 不合法，未修改。\n";
        return;
    }
    this->isbn = isbn;
}

void Book::setPublisher(const std::string& publisher) {
    if (publisher.empty()) {
        std::cerr << "[警告] setPublisher: 出版社不能为空，未修改。\n";
        return;
    }
    this->publisher = publisher;
}

void Book::setPrice(double price) {
    if (price < 0.0) {
        std::cerr << "[警告] setPrice: 价格不能为负(收到 " << price
                  << ")，未修改。\n";
        return;
    }
    this->price = price;
}

void Book::setPages(int pages) {
    if (pages <= 0) {
        std::cerr << "[警告] setPages: 页数必须大于 0(收到 " << pages
                  << ")，未修改。\n";
        return;
    }
    this->pages = pages;
}

void Book::setStock(int stock) {
    if (stock < 0) {
        std::cerr << "[警告] setStock: 库存数量不能为负(收到 " << stock
                  << ")，未修改。\n";
        return;
    }
    this->stock = stock;
}

void Book::setAll(const std::string& name,
                  const std::string& isbn,
                  const std::string& publisher,
                  double             price,
                  int                pages,
                  int                stock) {
    // 逐字段校验，非法则保留原值并给出提示（合法才更新该字段）
    if (name.empty()) {
        std::cerr << "[警告] setAll: 名称不能为空，未修改 name。\n";
    } else {
        this->name = name;
    }
    if (!validateISBN(isbn)) {
        std::cerr << "[警告] setAll: ISBN \"" << isbn
                  << "\" 不合法，未修改 isbn。\n";
    } else {
        this->isbn = isbn;
    }
    if (publisher.empty()) {
        std::cerr << "[警告] setAll: 出版社不能为空，未修改 publisher。\n";
    } else {
        this->publisher = publisher;
    }
    if (price < 0.0) {
        std::cerr << "[警告] setAll: 价格不能为负，未修改 price。\n";
    } else {
        this->price = price;
    }
    if (pages <= 0) {
        std::cerr << "[警告] setAll: 页数必须大于 0，未修改 pages。\n";
    } else {
        this->pages = pages;
    }
    if (stock < 0) {
        std::cerr << "[警告] setAll: 库存数量不能为负，未修改 stock。\n";
    } else {
        this->stock = stock;
    }
}

// ================= 获取操作 =================
// 一行式 getter：直接返回成员。const 保证不修改对象。

std::string Book::getName()      const { return name; }
std::string Book::getISBN()      const { return isbn; }
std::string Book::getPublisher() const { return publisher; }
double      Book::getPrice()     const { return price; }
int         Book::getPages()     const { return pages; }
int         Book::getStock()     const { return stock; }

// 库存数量 > 0 即表示可借
bool Book::isAvailable() const {
    return stock > 0;
}

// ================= 输出操作 =================

void Book::display(std::ostream& os) const {
    os << "-------------------- 图书信息 --------------------\n";
    os << std::left;   // 后续输出左对齐（对本程序的字符串标签影响较小，主要为格式统一）
    os << "  图书名称 : " << name      << "\n";
    os << "  ISBN     : " << isbn      << "\n";
    os << "  出版社   : " << publisher << "\n";
    // fixed + setprecision(2)：价格以定点小数显示，固定保留 2 位小数
    os << "  价格     : " << std::fixed << std::setprecision(2) << price << " 元\n";
    os << "  页数     : " << pages     << " 页\n";
    // 三元运算符根据 isAvailable() 的结果显示不同的中文状态
    os << "  馆藏数量 : " << stock     << " 册"
       << "（" << (isAvailable() ? "可借" : "已借完") << "）\n";
    os << "-------------------------------------------------\n";
}

// ============================================================
// ISBN 合法性验证（静态函数，不依赖任何 Book 对象）
// ------------------------------------------------------------
// 总体思路：
//   第 1 步：去掉输入中的连字符 '-' 和空格，只保留有效字符；
//   第 2 步：按长度分流 —— 10 位按 ISBN-10 规则，13 位按 ISBN-13
//            规则，其余长度一律非法。
//
// 【ISBN-10 规则】
//   前 9 位必须是数字，第 10 位可以是数字或 X（X 代表数值 10）。
//   从左到右各位的权值依次为 10,9,8,...,1，
//   加权总和能被 11 整除即为合法。
//
// 【ISBN-13 规则】
//   13 位必须全部是数字。从左到右（从 1 开始数）：
//   奇数位权值为 1，偶数位权值为 3，
//   含校验位在内的加权总和能被 10 整除即为合法。
//   （代码中下标 i 从 0 开始，所以 i 为偶数时恰好对应奇数位、权值 1。）
// ============================================================
bool Book::validateISBN(const std::string& isbn) {
    // 1. 去除连字符和空格，得到纯字符
    std::string s;
    for (char c : isbn) {                 // 范围 for：逐个取出字符串中的字符
        if (c == '-' || c == ' ') continue; // 跳过分隔符，不放入结果串
        s.push_back(c);
    }

    const size_t n = s.size();
    if (n == 10) {
        // ---------- ISBN-10 ----------
        int sum = 0;
        // 处理前 9 位：必须全部为数字
        for (size_t i = 0; i < 9; ++i) {
            char c = s[i];
            if (c < '0' || c > '9') return false;  // 非数字字符，立即判非法
            // (10-i) 即权值 10,9,...,2；(c-'0') 把字符数字转成整数值
            sum += static_cast<int>(10 - i) * (c - '0');
        }
        // 处理末位：数字按其值，X/x 按 10 处理
        char last = s[9];
        int check;
        if (last == 'X' || last == 'x') {
            check = 10;
        } else if (last >= '0' && last <= '9') {
            check = last - '0';
        } else {
            return false;                  // 末位是其他非法字符
        }
        sum += check;                      // 末位权值为 1
        return (sum % 11) == 0;            // 能被 11 整除即合法
    }

    if (n == 13) {
        // ---------- ISBN-13 ----------
        int sum = 0;
        for (size_t i = 0; i < 13; ++i) {
            char c = s[i];
            if (c < '0' || c > '9') return false;  // 13 位必须全为数字
            int digit = c - '0';
            // 下标偶数（第1,3,5,...位）乘 1，奇数（第2,4,6,...位）乘 3
            sum += (i % 2 == 0) ? digit : digit * 3;
        }
        return (sum % 10) == 0;            // 能被 10 整除即合法
    }

    // 既不是 10 位也不是 13 位，非法
    return false;
}
