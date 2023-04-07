document.body.addEventListener('click', e => {
    if (e.target && e.target.nodeName == 'A') {
        e.preventDefault()
    }
})

function touchStartHandler(event) {
    fetch('/update?action=' + event.target.dataset.direction)
}

function touchEndHandler(event) {
    fetch('/update?action=s')
}

document.querySelectorAll('.control').forEach((item) => {
    item.addEventListener('touchstart', touchStartHandler)
})

document.querySelectorAll('.control').forEach((item) => {
    item.addEventListener('touchend', touchEndHandler)
})

var speedSettings = document.querySelectorAll(
    'input[type=radio][name="speed-settings"]'
)
speedSettings.forEach(radio =>
    radio.addEventListener('change', e => {
        fetch('/update?action=' + e.target.value)
    })
)

const pad = n => {
    if (n < 10) {
        return '0' + n.toString()
    }
    return n
}

const dEl = document.querySelector('input[type="date"]')
const tEl = document.querySelector('input[type="time"]')
let dt = new Date()
const tick = () => {
    dt.setSeconds(dt.getSeconds() + 1)
    dEl.value = dt.getFullYear().toString() + '-' + pad(dt.getMonth()) + '-' + pad(dt.getDate())
    tEl.value = pad(dt.getHours()) + ':' + pad(dt.getMinutes()) + ':' + pad(dt.getSeconds())
}
let tickI = setInterval(tick, 1000)

fetch('/datetime').then(response => {
    return response.text()
}).then(text => {
    dt = new Date(text)
})

dEl.addEventListener('focus', () => {
    clearInterval(tickI)
}, false)
dEl.addEventListener('blur', () => {
    tickI = setInterval(tick, 1000)
}, false)
dEl.addEventListener('input', () => {
    const d = new Date(dEl.value + 'T00:00:00')
    dt.setFullYear(d.getFullYear())
    dt.setMonth(d.getMonth())
    dt.setDate(d.getDate())
}, false)

tEl.addEventListener('focus', () => {
    clearInterval(tickI)
}, false)
tEl.addEventListener('blur', () => {
    tickI = setInterval(tick, 1000)
}, false)
tEl.addEventListener('input', () => {
    let v = tEl.value
    if ((v.match(/\:/g) || []).length < 2) {
        v += ':00'
    }
    const t = new Date('1970-01-01T' + v)
    dt.setHours(t.getHours())
    dt.setMinutes(t.getMinutes())
    dt.setSeconds(t.getSeconds())
}, false)
