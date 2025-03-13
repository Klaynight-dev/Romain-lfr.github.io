function showDetails(year) {
    // Hide the menu
    document.getElementById('menu').classList.add('hidden');
  
    // Show the details and add the "expanded" class
    const detailsContainer = document.getElementById('details');
    detailsContainer.classList.remove('hidden');
    detailsContainer.classList.add('expanded');

    // Update content based on the selected year
    document.getElementById('selected-year').innerText = `${year}`;
  
    // Placeholder data for demonstration
    const data = {
        '2008': { discipline: 'Judo', image: './images/2008.png', medals: [{ type: 'Bronze', sport: 'Individuel + 100kg     (10 - 0)' }] },
        '2012': { discipline: 'Judo', image: './images/2012.png', medals: [{ type: 'Or', sport: 'Individuel + 100kg     (10 - 0)' }] },
        '2016': { discipline: 'Judo', image: './images/2016.png', medals: [{ type: 'Or', sport: 'Individuel + 100kg     (10 - 0)' }] },
        '2020': {
            discipline: 'Judo',
            image: './images/2020.png',
            medals: [
                { type: 'Bronze', sport: 'Individuel + 100kg    (10 - 0)' },
                { type: 'Or', sport: 'Equipe mixte   (4 - 1)' },
            ],
        },
        '2024': {
            discipline: 'Judo',
            image: './images/2024.png',
            medals: [
                { type: 'Or', sport: 'Individuel + 100kg (10 - 0)' },
                { type: 'Or', sport: 'Equipe mixte (4 - 3)' },
            ],
        },
    };
  
    const yearData = data[year];
  
    // Update discipline
    document.getElementById('discipline-container').innerText = `${yearData.discipline}`;
  
    // Update image
    document.getElementById('image').src = yearData.image;
  
    // Update medals
    const medalsList = document.getElementById('medals-list');
    medalsList.innerHTML = ''; // Clear previous content
  
    yearData.medals.forEach((medal) => {
        const listItem = document.createElement('li');
        listItem.innerHTML = `
        <span><strong>${medal.type}</strong> - ${medal.sport}</span>
        <img src="https://via.placeholder.com/30?text=${medal.type}" alt="${medal.type}" />`;
        medalsList.appendChild(listItem);
        }
    );

}
  
function goBack() {
    // Hide the details view and remove the "expanded" class
    const detailsContainer = document.getElementById('details');
    detailsContainer.classList.add('hidden');
    detailsContainer.classList.remove('expanded');
  
    // Show the menu
    document.getElementById('menu').classList.remove('hidden');
}
  