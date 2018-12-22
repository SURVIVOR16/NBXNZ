

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


const Ye = date => {
  const Ye = date.getFullYear()
  return Ye
} 
const Mo = date => {
  const Mo = date.getMonth() + 1
  return Mo
} 
const Da = date => {
  const Da = date.getDate()
  return Da
} 
const ho = date => {
  const ho = date.getHours()
  return ho
} 
const mi = date => {
  const mi = date.getMinutes()
  return mi
} 
const se = date => {
  const se = date.getSeconds()
  return se
} 
const formatNumber = n => {

  n = n.toString()

  return n[1] ? n : '0' + n

}
module.exports = {
  formatDate: formatDate,
  getDates: getDates,
  dateLater: dateLater,
  Ye: Ye,
  Mo: Mo,
  Da: Da,
  ho: ho,
  mi: mi,
  se: se,

}



