//
//  CImageQuery.cpp
//  MLB-mobile
//
//  Created by Kristina Theroux on 2/13/20.
//

#include "CImageQuery.h"

CImageQuery::CImageQuery()
: CJsonQuery()
{

}

CImageQuery::~CImageQuery()
{
   
}

bool CImageQuery::GetImage(const char* url)
{
    return SendQuery(url);
}
