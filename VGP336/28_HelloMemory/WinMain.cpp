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

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 100);

    std::vector<Student*> students;
    //test New
    for (uint32_t i = 0; i < 100; ++i)
    {
        Student* newStudent = studentPool.New();
        newStudent->Log();
        students.push_back(newStudent);
    }

    //test Deletion
    for (uint32_t i = 0; i < 60;++i)
    {
        Student* student = students.back();
        studentPool.Delete(student);
        students.pop_back();
    }

    //test Addition
    for (uint32_t i = 0;i < 55;++i)
    {
        Student* student = studentPool.New("Lalalalala");
        student->Log();
        students.push_back(student);
    }

    //clear all
    for (Student* student : students)
    {
        studentPool.Delete(student);
    }
    students.clear();

    return (0);
}