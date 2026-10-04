// ============================================================
// Student.cpp —— Student 类（学生/借阅者）的成员函数实现
// ------------------------------------------------------------
// 类的声明与关系说明见 Student.h。
// 借还书的核心思想：
//   学生侧的 borrowedBooks 数组保存所借图书的"信息副本"，
//   而图书馆侧真实的 Book 对象通过指针传入，库存的增减都
//   经由指针作用在原对象上，保证全系统库存数据唯一、一致。
// ============================================================

#include "Student.h"

// ================= 构造函数 =================

// 默认构造：借阅数量必须初始化为 0，否则数组计数是随机值
Student::Student()
    : id(""), name(""), major(""), borrowedCount(0)
{
}

// 带参构造：新生尚未借书，borrowedCount 固定初始化为 0
Student::Student(const std::string& id,
                 const std::string& name,
                 const std::string& major)
    : id(id), name(name), major(major), borrowedCount(0)
{
}

// ================= 修改操作 =================

void Student::setId(const std::string& id) {
    this->id = id;
}

void Student::setName(const std::string& name) {
    this->name = name;
}

void Student::setMajor(const std::string& major) {
    this->major = major;
}

// ================= 获取操作 =================

std::string Student::getId()   const { return id; }
std::string Student::getName() const { return name; }
std::string Student::getMajor() const { return major; }
int  Student::getBorrowedCount() const { return borrowedCount; }
int  Student::getMaxBorrow()    const { return MAX_BORROW; }

// ============================================================
// 借书（依赖关系的核心函数）
// ------------------------------------------------------------
// 形参 Book* book 是图书馆中真实图书的地址。
// 校验按"先学生、后图书"的顺序进行，任何一关不通过都直接
// 返回 false 并提示原因；只有全部通过才真正执行借出台账变动。
// ============================================================
bool Student::borrowBook(Book* book) {
    // 1. 指针有效性判断：防止对空指针解引用导致程序崩溃
    if (book == nullptr) {
        std::cout << "[借书失败] 传入的图书对象为空。\n";
        return false;
    }

    // 2. 判断学生状态：是否已达借阅上限
    if (borrowedCount >= MAX_BORROW) {
        std::cout << "[借书失败] " << name << " 已达借阅上限("
                  << MAX_BORROW << "本)，无法再借《" << book->getName() << "》。\n";
        return false;
    }

    // 3. 判断学生状态：是否已借过同一本书（按 ISBN 识别，不重复借）
    if (hasBorrowed(book->getISBN())) {
        std::cout << "[借书失败] " << name << " 已借过《" << book->getName()
                  << "》，不可重复借阅。\n";
        return false;
    }

    // 4. 判断书的状态：图书馆藏是否 > 0
    if (!book->isAvailable()) {
        std::cout << "[借书失败] 《" << book->getName()
                  << "》已无库存，无法借阅。\n";
        return false;
    }

    // 5. 两方面状态都允许，借书成功，同步更新双方台账：
    //    (1) 修改书的状态：图书馆藏 -1
    //        通过指针调用，改动的是 main 中的原图书对象，而非副本
    book->setStock(book->getStock() - 1);
    //    (2) 修改学生的状态：把这本书的信息副本存入已借数组末尾，
    //        然后借阅计数 +1（计数同时指向下一个空位）
    borrowedBooks[borrowedCount] = *book;
    borrowedCount++;

    std::cout << "[借书成功] " << name << " 借阅了《" << book->getName()
              << "》，当前已借 " << borrowedCount << "/" << MAX_BORROW << " 本。\n";
    return true;
}

// ============================================================
// 还书：借书的逆向操作
// ------------------------------------------------------------
// 步骤：空指针检查 → 在已借数组中按 ISBN 查找 → 找不到则失败
//       → 找到则删除该条记录（后续元素前移）→ 计数 -1
//       → 通过指针把图书馆藏 +1。
// ============================================================
bool Student::returnBook(Book* book) {
    if (book == nullptr) {
        std::cout << "[还书失败] 传入的图书对象为空。\n";
        return false;
    }

    // 在已借数组中线性查找该书（按 ISBN 匹配）
    int foundIndex = -1;   // -1 表示未找到
    for (int i = 0; i < borrowedCount; ++i) {
        if (borrowedBooks[i].getISBN() == book->getISBN()) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        std::cout << "[还书失败] " << name << " 未借阅《" << book->getName() << "》。\n";
        return false;
    }

    // 从数组中删除该记录：把它后面的元素依次前移一位覆盖，
    // 这样有效记录始终连续地占据 [0, borrowedCount)
    for (int i = foundIndex; i < borrowedCount - 1; ++i) {
        borrowedBooks[i] = borrowedBooks[i + 1];
    }
    borrowedCount--;

    // 恢复书的库存（通过指针修改图书馆侧的原对象）
    book->setStock(book->getStock() + 1);

    std::cout << "[还书成功] " << name << " 归还了《" << book->getName()
              << "》，当前已借 " << borrowedCount << "/" << MAX_BORROW << " 本。\n";
    return true;
}

// 是否已借过某 ISBN 的书：遍历有效区间做字符串相等比较
bool Student::hasBorrowed(const std::string& isbn) const {
    for (int i = 0; i < borrowedCount; ++i) {
        if (borrowedBooks[i].getISBN() == isbn) {
            return true;   // 找到一条匹配即可
        }
    }
    return false;
}

// ================= 输出操作 =================

// 打印学生基本信息与当前借阅进度
void Student::display(std::ostream& os) const {
    os << "==================== 学生信息 ====================\n";
    os << "  学号     : " << id      << "\n";
    os << "  姓名     : " << name    << "\n";
    os << "  专业     : " << major   << "\n";
    os << "  已借数量 : " << borrowedCount << " / " << MAX_BORROW << "\n";
    os << "==================================================\n";
}

// 逐条打印已借图书清单；一本都没借时给出空清单提示
void Student::displayBorrowedBooks(std::ostream& os) const {
    os << "----- " << name << " 的借阅清单 -----\n";
    if (borrowedCount == 0) {
        os << "  （当前无借阅图书）\n";
    } else {
        for (int i = 0; i < borrowedCount; ++i) {
            // 序号从 1 开始显示更符合阅读习惯，故用 i + 1
            os << "  " << (i + 1) << ". 《" << borrowedBooks[i].getName()
               << "》  ISBN: " << borrowedBooks[i].getISBN() << "\n";
        }
    }
    os << "------------------------------------\n";
}
