

const formatDate = date => {

  const year = date.getFullYear()

  const month = date.getMonth() + 1

  const day = date.getDate()

  return [year, month, day].map(formatNumber).join('-')

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


const Y = date => {
  const Y = date.getFullYear()
  return Y
} 
const M = date => {
  const M = date.getMonth() + 1
  return M
} 
const D = date => {
  const D = date.getDate()
  return D
} 
const W = date => {
  const W = date.getWeekday()
  return W
} 
const h = date => {
  const h = date.getHours()
  return h
} 
const m = date => {
  const m = date.getMinutes()
  return m
} 
const s = date => {
  const s = date.getSeconds()
  return s
} 
const formatNumber = n => {

  n = n.toString()

  return n[1] ? n : '0' + n

}
module.exports = {
  formatDate: formatDate,
  getDates: getDates,
  dateLater: dateLater,
  Y: Y,
  M: M,
  D: D,
  h: h,
  m: m,
  s: s,

}


