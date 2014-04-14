/*
  * (c) Copyright 2014 Franco Eusébio Garcia
  *
  * This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */

#include <cstdio>
#include <functional>
#include <vector>

// Any XML library to read the XML resources.
#include "tinyxml2.h"

typedef unsigned int EventData;

typedef std::function<void(EventData*)> Delegate;
typedef std::vector<Delegate> DelegateCollection;
typedef void (*Function)(EventData*);

typedef tinyxml2::XMLDocument XMLDocument;
typedef tinyxml2::XMLElement XMLElement;

// A few event handlers.
void Foo(EventData* pEventData)
{
    printf("Foo() - %d\n", *pEventData);
}

void Bar(EventData* pEventData)
{
    printf("Bar() - %d\n", *pEventData);
}

void Qux(EventData* pEventData)
{
    printf("Qux() - %d\n", *pEventData);
}

// Register the delegate if it is enabled in the XML resource.
void RegisterDelegateIfEnabled(DelegateCollection* pDelegates,
                      Function function,
                      XMLElement* pXMLElement)
{
    bool bIsEnabled = false;
    pXMLElement->QueryBoolAttribute("enable", &bIsEnabled);
    if (bIsEnabled)
    {
        Delegate delegate(function);
        pDelegates->push_back(delegate);
    }
}

int main()
{
    DelegateCollection delegates;

    XMLDocument xmlDocument;
    xmlDocument.LoadFile("handlers.xml");
    // Fetch the root element.
    XMLElement* pXMLElement = xmlDocument.FirstChildElement("Handlers");

    const unsigned int kStringSize = 51u;
    const unsigned int kTotalHandlers = 3u;
    // Function handlers.
    const Function kHandlers[kTotalHandlers] = {Foo, Bar, Qux};
    // The names of the elements in the XML resource.
    const char kElements[kTotalHandlers][kStringSize] = {"Foo", "Bar", "Qux"};
    // Register the handlers which are enabled in the XML resource.
    for (unsigned int handlerIndex = 0u; handlerIndex < kTotalHandlers; ++handlerIndex)
    {
        RegisterDelegateIfEnabled(&delegates,
                                  kHandlers[handlerIndex],
                                  pXMLElement->FirstChildElement(kElements[handlerIndex]));
    }

    unsigned int totalDelegates = 0u;
    for (const auto& delegate : delegates)
    {
        ++totalDelegates;
        delegate(&totalDelegates);
    }

    return 0;
}
