#include "pch.h"
#include "sub_40a4e5.h"
#include "../global-variables.h"
#include "sub_40a380.h"

void sub_40a4e5(
)
{
    if (global_db_40a551 != TRUE)
    {

        sub_40a380();

        //TODO: ���� -nohide, -nosleep, -altcolor �����Ĵ���
        global_db_40a551 = TRUE;
    }
}
