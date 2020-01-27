//
// Created by Kristina Theroux on 1/24/20.
//
void AdvanceDate(int& month, int& day, int& year)
{
    ++day;
    if( day < 28 )
        return;
    
    if( month ==1 || month == 3 || month==5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        if( day > 31)
        {
            day = 1;
            ++month;
            if( month > 12 )
            {
                ++year;
                month = 1;
            }
        }
    }
    else if( month == 2 )
    {
        int max = year % 4 == 0 ? 29 : 28;
        if( day > max)
        {
            month = 3;
            day = 1;
        }
    }
    else
    {
        if( day > 30)
        {
            ++month;
            day = 1;
        }
    }
}

void DecrementDate(int& month, int& day, int& year)
{
    --day;
    if( day > 0)
        return;
    
    //fall back a year
    if( month == 1)
    {
        --year;
        month = 12;
        day = 31;
        return;
    }
    
    //fall back a month
    --month;
    if( month ==1 || month == 3 || month==5 || month == 7 || month == 8 || month == 10)
    {
        day = 31;
    }
    else if( month == 2 )
    {
        int max = year % 4 == 0 ? 29 : 28;
        day = max;
    }
    else
    {
        day = 30;
    }
}


const char* TeamNameFromID(int id)
{
    switch(id)
    {
        case 160:
            return "National League";
        case 159:
            return "American League";
        case 158:
            return "Milwaukee";
        case 147:
            return "NY Yankees";
        case 146:
            return "Miami";
        case 145:
            return "Chi White Sox";
        case 144:
            return "Atlanta";
        case 143:
            return "Philadelphia";
        case 142:
            return "Minnesota";
        case 141:
            return "Toronto";
        case 140:
            return "Texas";
        case 139:
            return "Tampa Bay";
        case 138:
            return "St Louis";
        case 137:
            return "San Francisco";
        case 136:
            return "Seattle";
        case 135:
            return "San Diego";
        case 134:
            return "Pittsburgh";
        case 133:
            return "Oakland";
        case 121:
            return "NY Mets";
        case 120:
            return "Washington";
        case 119:
            return "LA Dodgers";
        case 118:
            return "Kansas City";
        case 117:
            return "Houston";
        case 116:
            return "Detroit";
        case 115:
            return "Colorado";
        case 114:
            return "Cleveland";
        case 113:
            return "Cincinnati";
        case 112:
            return "Chi Cubs";
        case 111:
            return "Boston";
        case 110:
            return "Baltimore";
        case 109:
            return "Arizona";
        case 108:
            return "LA Angels";
        dafault:
            return "";

    }
    
    return "";
}

