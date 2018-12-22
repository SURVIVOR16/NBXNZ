

const formatDate = date => {

  const year = date.getFullYear()

  const month = date.getMonth() + 1

  const day = date.getDate()

  return [year, month, day].map(formatNumber).join('-')

}
const formatTime = date => {

  const Y = date.getFullYear()

  const M = date.getMonth() + 1

  const D = date.getDate()

  const h = date.getHours()

  const m = date.getMinutes()

  const s = date.getSeconds()



  return [Y, M, D, h, m, s].map(formatNumber).join('/') + '/'
}

function getDates(days, todate) {

  

  for (var i = 0; i < days; i++) {
    var dateObj = dateLater(todate, i);
  }

  return dateObj;

}

function dateLater(dates, later) {

  let dateObj = {};

  let show_day = new Array(7, 1, 2, 3, 4, 5,6);

  let date = new Date(dates);

  date.setDate(date.getDate() + later);

  let day = date.getDay();

  let yearDate = date.getFullYear();

  let month = ((date.getMonth() + 1) < 10 ? ("0" + (date.getMonth() + 1)) : date.getMonth() + 1);

  let dayFormate = (date.getDate() < 10 ? ("0" + date.getDate()) : date.getDate());


  dateObj = show_day[day];

  return dateObj;

}


const formatNumber = n => {

  n = n.toString()

  return n[1] ? n : '0' + n

}
module.exports = {
  formatDate: formatDate,
  formatTime: formatTime,
  getDates: getDates,
  dateLater: dateLater,

}




