



var util = require('../../utils/util.js');

Page({

  data: {
    imageSrc: '../../image/background.jpg'

  },

  getTime: function () {

    var Y = util.Y(new Date());
    var M = util.M(new Date());
    var D = util.D(new Date());
    var h = util.h(new Date());
    var m = util.m(new Date());
    var s = util.s(new Date());

    let time = util.formatDate(new Date());
    let date = util.getDates(1, time);
    var W = date

    this.setData({

      Y: Y,
      M: M,
      D: D,
      W: W,
      h: h,
      m: m,
      s: s,


    })
    this.sendCmd(this.data.Y, this.data.M, this.data.D, this.data.W, this.data.h, this.data.m, this.data.s, this.data.hour, this.data.minute, this.data.on);

    console.log(Y, M, D, W, h, m, s)

  },


  searchBox: function (e) {
    const that = this;
    let hour, minute, on, h1, m1;
    that.setData({
      hour: e.detail.value.sethour,
      minute: e.detail.value.setminute,
      on: e.detail.value.onoroff,
    })
    this.sendCmd(this.data.Y, this.data.M, this.data.D, this.data.W, this.data.h, this.data.m, this.data.s, this.data.hour, this.data.minute, this.data.on);
  },




  sendCmd: function (Y, M, D, W, h, m, s, hour, minute, on) {
    var _this = this;
    wx.request({
      url: 'https://api.heclouds.com/devices/502965435/datapoints?type=3',
      header: {
        'api-key': '51g747=BYDNYa9=8CV0dQmeT120=',
      },
      method: 'POST',
      data: {
        Y, M, D, W, h, m, s,
        minute,
        hour, on
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
  goToTalkPage: function (param) {
    const requestTask = wx.request({

      url: 'https://api.heclouds.com/devices/502965435/datapoints?datastream_id=Light,Temperature,Humidity&limit=15',

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
