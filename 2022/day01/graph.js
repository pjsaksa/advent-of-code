let dragBegin = null
let fetchPending = false
let hoverBar = null
let showLocked = false
let shownBar = null
let sorted = false
let uiCanvas = null

function setSorted(newSorted) {
	sorted = newSorted

	const img = document.querySelector("#view1 > img")

	if (sorted) {
		img.src = "/01/graph?sorted=true"
	} else {
		img.src = "/01/graph"
	}

	hlClear()
}

function hlClear() {
	const ctx = uiCanvas.getContext("2d")

	ctx.clearRect(
		0,
		0,
		uiCanvas.width,
		uiCanvas.height)

	const info = document.querySelector("#info")
	if (info) {
		info.innerHTML = ""
		info.hidden = true
	}

	hoverBar = null
	shownBar = null
	dragBegin = null
	showLocked = false
}

function hlBar(range, data) {
	const ctx = uiCanvas.getContext("2d")

	ctx.clearRect(
		0,
		0,
		uiCanvas.width,
		uiCanvas.height)

	ctx.fillStyle = "#40b0f0"
	for (let idx = 0; idx < data.length; idx++) {
		ctx.fillRect(
			data[idx].bar*3 + 1,
			uiCanvas.height - data[idx].barHeight,
			2,
			data[idx].barHeight)
	}

	shownBar = range

	//

	const info = document.querySelector("#info")
	if (info) {
		let infoContent = "<button id=\"unlock\" onclick=\"hlClear()\" hidden>Clear selection</button>\n"
		let totalCalories = 0

		infoContent += "<ul>\n"
		for (let idx = 0; idx < data.length; idx++) {
			totalCalories += data[idx].totalCalories

			infoContent += "<li>Elf #" + (data[idx].elf+1) + " : " + data[idx].totalCalories + " = "
			for (let m = 0; m < data[idx].meals.length; m++) {
				if (m > 0) {
					infoContent += " + "
				}
				infoContent += data[idx].meals[m]
			}
			infoContent += "</li>\n"
		}

		if (data.length > 1) {
			infoContent += "<li>These " + data.length + " elves have " + totalCalories + " calories in total</li>\n"
		}

		infoContent += "</ul>\n"

		//

		info.innerHTML = infoContent
		info.hidden = false
	}
}

function hoverEvent(e) {
	if (event.type !== "mousemove"
	    || showLocked)
	{
		return
	}
	event.preventDefault()

	//

	if (event.offsetX >= 0)
	{
		const bar = Math.floor(event.offsetX / 3)

		if (bar >= 0
		    && bar < %d
		    && bar !== hoverBar)
		{
			hoverBar = bar

			//

			if (dragBegin !== null)
			{
				let dragRange
				if (dragBegin < hoverBar) {
					dragRange = dragBegin + "_" + hoverBar
				} else if (dragBegin > hoverBar) {
					dragRange = hoverBar + "_" + dragBegin
				} else {
					dragRange = hoverBar
				}

				fetchBar(dragRange)
			} else {
				fetchBar(hoverBar)
			}
		}
	}
}

function fetchBar(bar) {
	if (bar === shownBar
	    || fetchPending)
	{
		return
	}
	fetchPending = true

	//

	const query = ("n="+bar) + (sorted ? "&sorted=true" : "")
	fetch("/01/api/info?" + query)
		.then((response) => response.json())
		.then((json) => {
			fetchPending = false
			hlBar(bar, json)
			if (hoverBar !== null && dragBegin === null) {
				fetchBar(hoverBar)
			}
		})
		.catch((error) => {
			fetchPending = false
			console.error("Error fetching info: " + error.message)
		})
}

function dragBeginEvent(event) {
	if (event.type !== "mousedown"
	    || event.button != 0
	    || showLocked)
	{
		return
	}
	event.preventDefault()

	//

	dragBegin = hoverBar
}

function dragEndEvent(event) {
	if (event.type !== "mouseup"
	    || event.button != 0
	    || dragBegin === null
	    || showLocked)
	{
		return
	}
	event.preventDefault()

	//

	dragBegin = null
	showLocked = true

	document.querySelector("#unlock").hidden = false
}

function init() {
	uiCanvas = document.querySelector("#view1 > .ui")
	const ctx = uiCanvas.getContext("2d")

	ctx.lineCap = "square"

	uiCanvas.addEventListener("mousemove", hoverEvent, { capture: true })
	uiCanvas.addEventListener("mouseleave", (event) => { if (!showLocked) hlClear() }, { capture: true })
	uiCanvas.addEventListener("mousedown", dragBeginEvent, { capture: true })
	uiCanvas.addEventListener("mouseup", dragEndEvent, { capture: true })
}
