




var util = require('../../utils/util.js');

Page({

  data: {
    imageSrc: '../../image/background.jpg'

  },

  getTime: function () {

    var time = util.formatTime(new Date());//获取日期时间
    let tim = util.formatDate(new Date());
    let date = util.getDates(1, tim);
    var We = date//通过上面两个util中的函数计算 获取星期

    this.setData({

      time: time,
      We: We,
    

    })
    //通过这个语句触发下面的sendCmd函数，向onenet发送数据流
    this.sendCmd(this.data.We, this.data.time,this.data.hour, this.data.minute, this.data.on);

    console.log(We)

  },

//这个函数实现对输入框数据的储存
  searchBox: function (e) {
    const that = this;
    let hour, minute, on;//分别表示闹钟的时，分，和开关
    that.setData({
      hour: e.detail.value.sethour,//把输入的值附到定义的变量里，进行储存
      minute: e.detail.value.setminute,
      on: e.detail.value.onoroff,
    })
     //通过这个语句触发下面的sendCmd函数，向onenet发送数据流
    this.sendCmd(this.data.We, this.data.time, this.data.hour, this.data.minute, this.data.on);
  },



//向onenet发送数据
  sendCmd: function (We, time,  hour, minute, on) {//要发送的数据流，注意顺序要与下面data中的对应
    var _this = this;
    wx.request({
      url: 'https://api.heclouds.com/devices/502965435/datapoints?type=3',//注意更改deviceID
      header: {
        'api-key': '51g747=BYDNYa9=8CV0dQmeT120=',
      },
      method: 'POST',
      data: {
        
        We, 
        time,
        minute,
        hour, 
        on
      },
      success: function (res) {
        console.log("success!!")

      },
      fail: function (res) {
        console.log("fail!!!")
      }
    })
  },
  bindViewTap: function () {
    wx.navigateTo({
      ukl: '../logs/logs'
    })
  },
  //接收硬件端发送到onenet的数据
  goToTalkPage: function (param) {
    const requestTask = wx.request({

      url: 'https://api.heclouds.com/devices/502965435/datapoints?datastream_id=Light,Temperature,Humidity&limit=15',
      //注意更改相应信息

      header: {

        'content-type': 'application/json',

        'api-key': '51g747=BYDNYa9=8CV0dQmeT120='

      },

      success: function (res) {

        //console.log(res.data)

        //拿到数据后保存到全局数据

        var app = getApp()

        app.globalData.temperature = res.data.data.datastreams[0]

        app.globalData.humidity = res.data.data.datastreams[2]

        //跳转到天气页面，根据拿到的数据绘图

        wx.navigateTo({
          url: '/pages/index/talkPage'
        })

      },



      fail: function (res) {

        console.log("fail!!!")

      },



      complete: function (res) {

        console.log("end")

      }

    })

  },


  /**
* 生命周期函数--监听页面加载
*/
  onLoad: function (options) {
  },
  /**
  * 生命周期函数--监听页面初次渲染完成
  */
  onReady: function () {
  },
  /**
  * 生命周期函数--监听页面显示
  */
  onShow: function () {
  },
  /**
  * 生命周期函数--监听页面隐藏
  */
  onHide: function () {
  },
  /**
  * 生命周期函数--监听页面卸载
  */
  onUnload: function () {
  },
  /**
  * 页面相关事件处理函数--监听用户下拉动作
  */
  onPullDownRefresh: function () {
  },
  /**
  * 页面上拉触底事件的处理函数
  */
  onReachBottom: function () {
  },
  /**
  * 用户点击右上角分享
  */
  onShareAppMessage: function () {
  }


})

