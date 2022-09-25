#include "MembersManageService.h"

MembersManageService::MembersManageService(ComDev *comDev)
{
    membersEntity = new MembersEntity();
    membersManagerState = CARD_READER;
    this->comDev = comDev;
    lcd = new LCD(new I2C("/dev/i2c-1", 0x27));
    lcd->WriteStringXY(0, 0, "READER MODE   ");
}

MembersManageService::~MembersManageService()
{

}

void MembersManageService::setComDev(ComDev *comDev)
{
    this->comDev = comDev;
}

void MembersManageService::updateStateEvent(std::string devName)
{
    switch (membersManagerState)
    {
    case CARD_READER:
        if(devName == "modeButton")
        {
            membersManagerState = CARD_REGISTER;
            lcd->WriteStringXY(0, 0, "REGISTER MODE");
            printf("change to CARD_REGISTER state\n");
        }
        break;
    
    case CARD_REGISTER:
        if(devName == "modeButton")
        {
            membersManagerState = CARD_SEARCH;
            lcd->WriteStringXY(0, 0, "SEARCH MODE   ");
            printf("change to CARD_SEARCH state\n");
            card_search();
        }
        break;

    case CARD_SEARCH:
        if(devName == "modeButton")
        {
            membersManagerState = CARD_READER;
            lcd->WriteStringXY(0, 0, "READER MODE   ");
            printf("change to CARD_READER state\n");
        }
        break;
    };
}

void MembersManageService::checkCardNumber(int *cardNum)
{
    switch (membersManagerState)
    {
    case CARD_READER:
        if(membersEntity->findMemberInfo(cardNum))
        {
            printf("Registered Member!\n");
            membersEntity->printMemberInfo(cardNum);
            comDev->sendData(cardNum);
            lcd->WriteStringXY(1, 0, "MEMBER        ");
        }
        else
        {
            printf("Not Registered Member!\n");
            lcd->WriteStringXY(1, 0, "NOT MEMBER    ");
        }
        break;
    
    case CARD_REGISTER:
        if(!membersEntity->findMemberInfo(cardNum))
        {
            lcd->WriteStringXY(1, 0, "REGISTERING");
            MemberInfo tempMember;
            char name[20];
            char address[40];
            int phNumber;
            char phoneNumber[15];
            tempMember.id = membersEntity->get_id();
            printf("이름을 입력해주세요\n");
            scanf("%s", &name);
            strcpy(tempMember.name, name);
            printf("주소를 입력해주세요\n");
            scanf("%s", &address);
            strcpy(tempMember.address, address);
            printf("010을 제외하고 8자리 전화번호를 숫자만 입력해주세요\n");
            scanf("%d", &phNumber);
            sprintf(phoneNumber, "010-%04d-%04d", (phNumber / 10000), (phNumber % 10000));
            strcpy(tempMember.phoneNumber, phoneNumber);
            memcpy(&(tempMember.cardNum), cardNum, sizeof(tempMember.cardNum));
            membersEntity->addMemberInfo(tempMember);
            std::cout << tempMember.id << " " << tempMember.name << " "
            << tempMember.address << " " <<
            tempMember.phoneNumber << " " <<
            tempMember.cardNum << " " << std::endl;
            printf("Member Registered!\n");
            lcd->WriteStringXY(1, 0, "REGISTERED ");
        }
        else
        {
            printf("Already Registered Member!\n");
            lcd->WriteStringXY(1, 0, "ALREADY MEMBER");
        }
        break;
    case CARD_SEARCH:
        get_card(cardNum);
    break;
    };
}
#if 0
void MembersManageService::card_search()
{
    char way[20];
    int id;
    // char name[20];
    // char address[40];
    int phNumber;
    char phoneNumber[15];
    lcd->WriteStringXY(1, 0, "SEARCHING...  ");
    printf("카드를 대주세요\n");
    printf("무엇으로 검색하시겠습니까?\n");
    printf("id / phone / card\n");
    scanf("%s", &way);
    if(strcmp(way, "id") == 0)
    {
        printf("id : ");
        scanf("%d", id);
        if(membersEntity->findMemberInfo(id))
        {
            membersEntity->printMemberInfo(id);
        }
        else
        {
            printf("등록되지 않은 id입니다.\n");
        }
    }
    else if(strcmp(way, "phone") == 0)
    {
        if(membersEntity->findMemberInfo(""))
        {
            membersEntity->printMemberInfo("");
        }
        else
        {
            printf("등록되지 않은 phone number입니다.\n");
        }
    }
    else if(strcmp(way, "card") == 0)
    {
        if(membersEntity->findMemberInfo(iner.cardNum))
        {
            membersEntity->printMemberInfo(iner.cardNum);
        }
        else
        {
            printf("등록되지 않은 card number입니다.\n");
        }
    }
    else
    {
        printf("철자를 확인하여 다시 입력해주십시오.\n");
        printf("id / phone / card\n");
        scanf("%s", &way);
    }
}
#else
void MembersManageService::card_search()
{
    char way[20];
    lcd->WriteStringXY(1, 0, "SEARCHING...  ");
    printf("카드를 올린 상태로 잠시 뒤 card를 입력해주세요\n");
    scanf("%s", &way);
    if(strcmp(way, "card") == 0)
    {
        if(membersEntity->findMemberInfo(iner.cardNum))
        {
            lcd->WriteStringXY(1, 0, "MEMBER        ");
            membersEntity->printMemberInfo(iner.cardNum);
        }
        else
        {
            printf("등록되지 않은 card number입니다.\n");
            lcd->WriteStringXY(1, 0, "NOT MEMBER    ");
        }
    }
    else
    {
        printf("철자를 확인하여 다시 입력해주십시오.\n");
        scanf("%s", &way);
    }
}
#endif
int* MembersManageService::get_card(int *cardNum)
{
    memcpy(&(iner.cardNum), cardNum, sizeof(iner.cardNum));
    return iner.cardNum;
}