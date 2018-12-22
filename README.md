硬件端
硬件.ino

微信端
utils包含（util.js 获取时间，日期，以及计算星期
         wxchart.js 绘制温湿曲线
         ）


pages中含有index和logs和test三个文件夹


     其中index（index.js  主界面包含 获取实时时间和设置闹钟并将数据发送到onenet  
                          还包括onenet上温湿度数据获取和温湿曲线按钮
               index.json
               index.wxml
               index.wass
               talkPage.js   点击温湿曲线按钮后跳转到该页面
               talkPage.json
               talkPage.wxml
               talkPage.wass）
               
               
               
               
         logs(logs.js   
              logs.json
              logs.wxml
              logs.wass)
              
              
              
              
         test(test.js    获取百度天气的界面
              test.json
              test.wxml
              test.wass)
              
              
              
              
libs中含(bmap-wx.js）   
         获取百度天气所需函数，包括位置定位以及天气数据的获取
