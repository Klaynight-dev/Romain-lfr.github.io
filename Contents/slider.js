let Index = 0;
const slides = document.querySelectorAll(".slide");
const points = document.querySelectorAll(".point");

function updateSlider(index = null) {
    if (index !== null) {
        Index = index;
    } else {
        Index = (Index + 1) % slides.length;
    }
    
    slides.forEach((slide, idx) => {
        slide.style.opacity = idx === Index ? "1" : "0";
        slide.style.transform = idx === Index ? "translateX(0)" : "translateX(100%)";
        slide.style.transition = "opacity 1s ease-in-out, transform 1s ease-in-out";
    });
    
    points.forEach((point, idx) => {
        point.classList.toggle("active", idx === Index);
    });
}

points.forEach((point, index) => {
    point.addEventListener("click", () => {
        updateSlider(index);
    });
});

setInterval(updateSlider, 5000);

updateSlider();