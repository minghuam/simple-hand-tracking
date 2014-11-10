//
//  gesturedata.h
//  simple_hand_tracking
//
//  http://depts.washington.edu/aimgroup/proj/dollar/ndollar.html
//

#ifndef simple_hand_tracking_gesturedata_h
#define simple_hand_tracking_gesturedata_h

#include "util.h"


// TODO: read from a file. This approach is just ugly
//--- Make it a struct so that everything defaults to public
struct SampleTemplates
{
    vector<vector<Vec2f> > raw_paths;
    
    SampleTemplates(){
        raw_paths.push_back(getGestureCircle());
        raw_paths.push_back(getGesturePigtail());
        raw_paths.push_back(getGestureTriangle());
        raw_paths.push_back(getGestureV());
    }
    
    vector<Vec2f> getGestureCircle()
    {
        vector<Vec2f> path;
        path.push_back(Vec2f(127,141));
        path.push_back(Vec2f(124,140));
        path.push_back(Vec2f(120,139));
        path.push_back(Vec2f(118,139));
        path.push_back(Vec2f(116,139));
        path.push_back(Vec2f(111,140));
        path.push_back(Vec2f(109,141));
        path.push_back(Vec2f(104,144));
        path.push_back(Vec2f(100,147));
        path.push_back(Vec2f(96,152));
        path.push_back(Vec2f(93,157));
        path.push_back(Vec2f(90,163));
        path.push_back(Vec2f(87,169));
        path.push_back(Vec2f(85,175));
        path.push_back(Vec2f(83,181));
        path.push_back(Vec2f(82,190));
        path.push_back(Vec2f(82,195));
        path.push_back(Vec2f(83,200));
        path.push_back(Vec2f(84,205));
        path.push_back(Vec2f(88,213));
        path.push_back(Vec2f(91,216));
        path.push_back(Vec2f(96,219));
        path.push_back(Vec2f(103,222));
        path.push_back(Vec2f(108,224));
        path.push_back(Vec2f(111,224));
        path.push_back(Vec2f(120,224));
        path.push_back(Vec2f(133,223));
        path.push_back(Vec2f(142,222));
        path.push_back(Vec2f(152,218));
        path.push_back(Vec2f(160,214));
        path.push_back(Vec2f(167,210));
        path.push_back(Vec2f(173,204));
        path.push_back(Vec2f(178,198));
        path.push_back(Vec2f(179,196));
        path.push_back(Vec2f(182,188));
        path.push_back(Vec2f(182,177));
        path.push_back(Vec2f(178,167));
        path.push_back(Vec2f(170,150));
        path.push_back(Vec2f(163,138));
        path.push_back(Vec2f(152,130));
        path.push_back(Vec2f(143,129));
        path.push_back(Vec2f(140,131));
        path.push_back(Vec2f(129,136));
        path.push_back(Vec2f(126,139));
        
        return path;
    }
    
    vector<Vec2f> getGesturePigtail()
    {
        vector<Vec2f> path;
        path.push_back(Vec2f(81,219));
        path.push_back(Vec2f(84,218));
        path.push_back(Vec2f(86,220));
        path.push_back(Vec2f(88,220));
        path.push_back(Vec2f(90,220));
        path.push_back(Vec2f(92,219));
        path.push_back(Vec2f(95,220));
        path.push_back(Vec2f(97,219));
        path.push_back(Vec2f(99,220));
        path.push_back(Vec2f(102,218));
        path.push_back(Vec2f(105,217));
        path.push_back(Vec2f(107,216));
        path.push_back(Vec2f(110,216));
        path.push_back(Vec2f(113,214));
        path.push_back(Vec2f(116,212));
        path.push_back(Vec2f(118,210));
        path.push_back(Vec2f(121,208));
        path.push_back(Vec2f(124,205));
        path.push_back(Vec2f(126,202));
        path.push_back(Vec2f(129,199));
        path.push_back(Vec2f(132,196));
        path.push_back(Vec2f(136,191));
        path.push_back(Vec2f(139,187));
        path.push_back(Vec2f(142,182));
        path.push_back(Vec2f(144,179));
        path.push_back(Vec2f(146,174));
        path.push_back(Vec2f(148,170));
        path.push_back(Vec2f(149,168));
        path.push_back(Vec2f(151,162));
        path.push_back(Vec2f(152,160));
        path.push_back(Vec2f(152,157));
        path.push_back(Vec2f(152,155));
        path.push_back(Vec2f(152,151));
        path.push_back(Vec2f(152,149));
        path.push_back(Vec2f(152,146));
        path.push_back(Vec2f(149,142));
        path.push_back(Vec2f(148,139));
        path.push_back(Vec2f(145,137));
        path.push_back(Vec2f(141,135));
        path.push_back(Vec2f(139,135));
        path.push_back(Vec2f(134,136));
        path.push_back(Vec2f(130,140));
        path.push_back(Vec2f(128,142));
        path.push_back(Vec2f(126,145));
        path.push_back(Vec2f(122,150));
        path.push_back(Vec2f(119,158));
        path.push_back(Vec2f(117,163));
        path.push_back(Vec2f(115,170));
        path.push_back(Vec2f(114,175));
        path.push_back(Vec2f(117,184));
        path.push_back(Vec2f(120,190));
        path.push_back(Vec2f(125,199));
        path.push_back(Vec2f(129,203));
        path.push_back(Vec2f(133,208));
        path.push_back(Vec2f(138,213));
        path.push_back(Vec2f(145,215));
        path.push_back(Vec2f(155,218));
        path.push_back(Vec2f(164,219));
        path.push_back(Vec2f(166,219));
        path.push_back(Vec2f(177,219));
        path.push_back(Vec2f(182,218));
        path.push_back(Vec2f(192,216));
        path.push_back(Vec2f(196,213));
        path.push_back(Vec2f(199,212));
        path.push_back(Vec2f(201,211));
        
        return path;
    }
    
    vector<Vec2f> getGestureTriangle()
    {
        vector<Vec2f> path;
        path.push_back(Vec2f(137,139));
        path.push_back(Vec2f(135,141));
        path.push_back(Vec2f(133,144));
        path.push_back(Vec2f(132,146));
        path.push_back(Vec2f(130,149));
        path.push_back(Vec2f(128,151));
        path.push_back(Vec2f(126,155));
        path.push_back(Vec2f(123,160));
        path.push_back(Vec2f(120,166));
        path.push_back(Vec2f(116,171));
        path.push_back(Vec2f(112,177));
        path.push_back(Vec2f(107,183));
        path.push_back(Vec2f(102,188));
        path.push_back(Vec2f(100,191));
        path.push_back(Vec2f(95,195));
        path.push_back(Vec2f(90,199));
        path.push_back(Vec2f(86,203));
        path.push_back(Vec2f(82,206));
        path.push_back(Vec2f(80,209));
        path.push_back(Vec2f(75,213));
        path.push_back(Vec2f(73,213));
        path.push_back(Vec2f(70,216));
        path.push_back(Vec2f(67,219));
        path.push_back(Vec2f(64,221));
        path.push_back(Vec2f(61,223));
        path.push_back(Vec2f(60,225));
        path.push_back(Vec2f(62,226));
        path.push_back(Vec2f(65,225));
        path.push_back(Vec2f(67,226));
        path.push_back(Vec2f(74,226));
        path.push_back(Vec2f(77,227));
        path.push_back(Vec2f(85,229));
        path.push_back(Vec2f(91,230));
        path.push_back(Vec2f(99,231));
        path.push_back(Vec2f(108,232));
        path.push_back(Vec2f(116,233));
        path.push_back(Vec2f(125,233));
        path.push_back(Vec2f(134,234));
        path.push_back(Vec2f(145,233));
        path.push_back(Vec2f(153,232));
        path.push_back(Vec2f(160,233));
        path.push_back(Vec2f(170,234));
        path.push_back(Vec2f(177,235));
        path.push_back(Vec2f(179,236));
        path.push_back(Vec2f(186,237));
        path.push_back(Vec2f(193,238));
        path.push_back(Vec2f(198,239));
        path.push_back(Vec2f(200,237));
        path.push_back(Vec2f(202,239));
        path.push_back(Vec2f(204,238));
        path.push_back(Vec2f(206,234));
        path.push_back(Vec2f(205,230));
        path.push_back(Vec2f(202,222));
        path.push_back(Vec2f(197,216));
        path.push_back(Vec2f(192,207));
        path.push_back(Vec2f(186,198));
        path.push_back(Vec2f(179,189));
        path.push_back(Vec2f(174,183));
        path.push_back(Vec2f(170,178));
        path.push_back(Vec2f(164,171));
        path.push_back(Vec2f(161,168));
        path.push_back(Vec2f(154,160));
        path.push_back(Vec2f(148,155));
        path.push_back(Vec2f(143,150));
        path.push_back(Vec2f(138,148));
        path.push_back(Vec2f(136,148));
        
        return path;
    }
    
    vector<Vec2f> getGestureV()
    {
        vector<Vec2f> path;
        path.push_back(Vec2f(89,164));
        path.push_back(Vec2f(90,162));
        path.push_back(Vec2f(92,162));
        path.push_back(Vec2f(94,164));
        path.push_back(Vec2f(95,166));
        path.push_back(Vec2f(96,169));
        path.push_back(Vec2f(97,171));
        path.push_back(Vec2f(99,175));
        path.push_back(Vec2f(101,178));
        path.push_back(Vec2f(103,182));
        path.push_back(Vec2f(106,189));
        path.push_back(Vec2f(108,194));
        path.push_back(Vec2f(111,199));
        path.push_back(Vec2f(114,204));
        path.push_back(Vec2f(117,209));
        path.push_back(Vec2f(119,214));
        path.push_back(Vec2f(122,218));
        path.push_back(Vec2f(124,222));
        path.push_back(Vec2f(126,225));
        path.push_back(Vec2f(128,228));
        path.push_back(Vec2f(130,229));
        path.push_back(Vec2f(133,233));
        path.push_back(Vec2f(134,236));
        path.push_back(Vec2f(136,239));
        path.push_back(Vec2f(138,240));
        path.push_back(Vec2f(139,242));
        path.push_back(Vec2f(140,244));
        path.push_back(Vec2f(142,242));
        path.push_back(Vec2f(142,240));
        path.push_back(Vec2f(142,237));
        path.push_back(Vec2f(143,235));
        path.push_back(Vec2f(143,233));
        path.push_back(Vec2f(145,229));
        path.push_back(Vec2f(146,226));
        path.push_back(Vec2f(148,217));
        path.push_back(Vec2f(149,208));
        path.push_back(Vec2f(149,205));
        path.push_back(Vec2f(151,196));
        path.push_back(Vec2f(151,193));
        path.push_back(Vec2f(153,182));
        path.push_back(Vec2f(155,172));
        path.push_back(Vec2f(157,165));
        path.push_back(Vec2f(159,160));
        path.push_back(Vec2f(162,155));
        path.push_back(Vec2f(164,150));
        path.push_back(Vec2f(165,148));
        path.push_back(Vec2f(166,146));
        
        return path;
    }
};

#endif
