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
        const angle = (idx - Index) * 120; // Chaque slide est décalée de 120° pour un triangle
        slide.style.transform = `rotateY(${angle}deg) translateZ(50vw)`;
        slide.style.opacity = idx === Index ? "1" : "0";
        slide.style.visibility = "visible"; // Toujours visible pour permettre la transition

        // Ajout d'un délai pour cacher complètement après la transition
        if (idx !== Index) {
            setTimeout(() => {
                slide.style.visibility = "hidden";
            }, 1000); // Correspond à la durée de la transition (1s)
        }

        slide.style.transition = "transform 1s ease-in-out, opacity 1s ease-in-out";
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