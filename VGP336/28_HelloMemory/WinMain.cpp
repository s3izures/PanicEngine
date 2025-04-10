#include <PanicEngine/Inc/PanicEngine.h>

using namespace PanicEngine;
using namespace PanicEngine::Core;

static int sUniqueId = 0;
class Student
{
public:
    Student() 
    {
        mId = ++sUniqueId;
        mName = "N/a";
    }

    Student(const std::string& name)
        : mName(name), mId(++sUniqueId)
    {

    }

    void Log()
    {
        LOG("Name: %s | Id: %d", mName.c_str(), mId);
    }

    void SetName(const std::string& name)
    {
        mName = name;
    }

private:
    int mId = 0;
    std::string mName;
};

class Cow
{
public:
    Cow()
    {
        mId = ++sUniqueId;
        mName = "Cow";
    }

    void Log()
    {
        LOG("Name: %s | Id: %d", mName.c_str(), mId);
    }

private:
    int mId = 0;
    std::string mName;
};

class Hay
{
public:
    Hay()
    {
        mId = ++sUniqueId;
        mName = "Hay";
    }

    void Log()
    {
        LOG("Name: %s | Id: %d", mName.c_str(), mId);
    }

private:
    int mId = 0;
    std::string mName;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 100);
    TypedAllocator cowPool = TypedAllocator<Cow>("CowPool", 8);
    TypedAllocator hayPool = TypedAllocator<Hay>("HayPool", 50);

    std::vector<Student*> students;
    std::vector<Cow*> cows;
    std::vector<Hay*> hays;

    //--- test New ----------------------------------
    for (uint32_t i = 0; i < 100; ++i)
    {
        Student* newStudent = studentPool.New();
        newStudent->Log();
        students.push_back(newStudent);

        if (i < 50)
        {
            Hay* newHay = hayPool.New();
            newHay->Log();
            hays.push_back(newHay);
        }

        if (i < 8)
        {
            Cow* newCow = cowPool.New();
            newCow->Log();
            cows.push_back(newCow);
        }
    }

    //--- test Deletion ----------------------------------
    for (uint32_t i = 0; i < 60;++i)
    {
        Student* student = students.back();
        studentPool.Delete(student);
        students.pop_back();
    }

    for (uint32_t i = 0; i < 5;++i)
    {
        Cow* cow = cows.back();
        cowPool.Delete(cow);
        cows.pop_back();

        Hay* hay = hays.back();
        hayPool.Delete(hay);
        hays.pop_back();
    }

    //--- test Addition ----------------------------------
    for (uint32_t i = 0;i < 55;++i)
    {
        Student* student = studentPool.New("Lalalalala");
        student->Log();
        students.push_back(student);
    }

    for (uint32_t i = 0;i < 2;++i)
    {
        Cow* cow = cowPool.New();
        cow->Log();
        cows.push_back(cow);

        Hay* hay = hayPool.New();
        hay->Log();
        hays.push_back(hay);
    }

    //--- clear all ----------------------------------
    for (Student* student : students)
    {
        studentPool.Delete(student);
    }
    students.clear();

    for (Cow* cow : cows)
    {
        cowPool.Delete(cow);
    }
    cows.clear();

    for (Hay* hay : hays)
    {
        hayPool.Delete(hay);
    }
    hays.clear();

    return (0);
}