//
//  CImageQuery.hpp
//  MLB-mobile
//
//  Created by Kristina Theroux on 2/13/20.
//

#ifndef CImageQuery_hpp
#define CImageQuery_hpp

#include <stdio.h>
#include "CJsonQuery.h"

class CImageQuery : public CJsonQuery
{
public:
    CImageQuery();
    virtual ~CImageQuery();
    
    bool GetImage(const char* url);
    
};

#endif /* CImageQuery_hpp */
