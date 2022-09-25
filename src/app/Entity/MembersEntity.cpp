#include "MembersEntity.h"

MembersEntity::MembersEntity()
{
    loadMembersData();
}

MembersEntity::~MembersEntity()
{

}

void MembersEntity::loadMembersData()
{
    int cnt = 0;
    fpDB = fopen("memberLists.bin", "r");
    if(fpDB == NULL)
    {
        fprintf(stderr, "file open error\n");
        return;
    }
    while(fread(&memberInfo, sizeof(MemberInfo), 1, fpDB))
    {
        vecMembersList.push_back(memberInfo);
        cnt++;
    }
    fclose(fpDB);
    printf("cnt : %d\n", cnt);
}

void MembersEntity::addMemberInfo(MemberInfo member)
{
    vecMembersList.push_back(member);
    printf("size : %d\n", (int)vecMembersList.size());
}

bool MembersEntity::deleteMemberInfo(int *cardNum)
{
    std::vector<MemberInfo>::iterator itrMember; //일종의 포인터 같은 것
    // itrMember = vecMembersList.begin(); //begin : vector의 제일 첫 번째 주소
    //iterator는 주소를 가졌으니 포인터변수와 거의 같다.
    for(itrMember = vecMembersList.begin(); itrMember != vecMembersList.end(); itrMember++) //vector의 마지막주소와 같지 않을 때까지 for문 실행
    {
        if(memcmp(itrMember->cardNum, cardNum, sizeof(itrMember->cardNum)) == 0) //if(itrMember->cardNum == cardNum)
        {
            vecMembersList.erase(itrMember);
            return true;
        }
    }
    return false;
}

bool MembersEntity::deleteMemberInfo(std::string name)
{
    ;
}

void MembersEntity::printMemberInfo(int index)
{
    if (index > (int)vecMembersList.size())
    {
        fprintf(stderr, "out of range member count!\n");
        return;
    }
    printf("%06d, %s, %s, %s, %02x-%02x-%02x-%02x-%02x\n",
           vecMembersList[index].id,
           vecMembersList[index].name,
           vecMembersList[index].address,
           vecMembersList[index].phoneNumber,
           vecMembersList[index].cardNum[0],
           vecMembersList[index].cardNum[1],
           vecMembersList[index].cardNum[2],
           vecMembersList[index].cardNum[3],
           vecMembersList[index].cardNum[4]
           );
}

void MembersEntity::printMemberInfo(std::string name)
{
    for(const auto &member : vecMembersList) //reference member
    {
        if(strcmp(member.name, name.c_str()) == 0)
        {
            printMemberInfo(member.id);
            return;
        }
    }
}

void  MembersEntity::printMemberInfo(int *cardNum)
{
    for(const auto &member : vecMembersList) //reference member
    {
        if(memcmp(member.cardNum, cardNum, sizeof(member.cardNum)) == 0)
        {
            // printMemberInfo(member.id);
            printf("%06d, %s, %s, %s, %02x-%02x-%02x-%02x-%02x\n",
            member.id,
            member.name,
            member.address,
            member.phoneNumber,
            member.cardNum[0],
            member.cardNum[1],
            member.cardNum[2],
            member.cardNum[3],
            member.cardNum[4]);
            return;
        }
    }
}

bool MembersEntity::findMemberInfo(int index)
{
    for(int i = 0; i<vecMembersList.size();i++) //reference member
    {
        if(vecMembersList[i].id == index)
        {
            return true;
        }
    }
    return false;
}



bool MembersEntity::findMemberInfo(int *cardNum)
{
    for(const auto &member : vecMembersList) //reference member
    {
        if(memcmp(member.cardNum, cardNum, sizeof(member.cardNum)) == 0)
        {
            return true;
        }
    }
    return false;
}

bool MembersEntity::findMemberInfo(std::string name)
{
    for(const auto &member : vecMembersList) //reference member
    {
        if(strcmp(member.name, name.c_str()) == 0)
        {
            printMemberInfo(member.id);
            return true;
        }
    }
    return false;
}

void MembersEntity::memoryToDB()
{
    fpDB = fopen("memberLists.bin", "w"); //덮어씌우기
    FILE *fpDB2 = fopen("memberLists.txt", "w"); //NULL
    if(fpDB == NULL)
    {
        fprintf(stderr, "file open error\n");
        return;
    }
    for(const auto &member : vecMembersList)
    {
        fwrite(&member, sizeof(member), 1, fpDB);
        fprintf(fpDB2, "%05d, %s\n", member.id, member.name);
    }
    fclose(fpDB);
    fclose(fpDB2);
    /*file open
    vecMembersList의 배열을 하나씩 읽고
    vecMembersList 값을 file에 vecMembersList 끝까지 write
    file close*/
}

int MembersEntity::get_id()
{
    int i;
    for(i = 1; i < (int)vecMembersList.size();i++)
    {
        if(vecMembersList[i].id == 0)
        {
            return i;
        }
    }
    return i+1;
}