#ifndef MEMBERSMANAGESERVICE_H
#define MEMBERSMANAGESERVICE_H

#pragma once
#include <string>
#include "MembersEntity.h"
#include "MemberInfo.h"
#include "ComDev.h"
#include "LCD.h"
#include "I2C.h"

enum {CARD_READER, CARD_REGISTER, CARD_SEARCH};

class MembersManageService
{
public:
    MembersManageService(ComDev *comDev);
    virtual ~MembersManageService();
    void setComDev(ComDev *comDev);
    void updateStateEvent(std::string devName);
    void checkCardNumber(int *cardNum);
    void card_search();
    int* get_card(int *cardNum);

private:
    int membersManagerState;
    MembersEntity *membersEntity;
    ComDev *comDev;
    LCD *lcd;
    MemberInfo iner;
};

#endif