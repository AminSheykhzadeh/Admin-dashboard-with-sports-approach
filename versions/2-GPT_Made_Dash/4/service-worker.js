self.addEventListener('install', (event) => {
  console.log('Service Worker installing.');
  // Perform install steps
});

self.addEventListener('fetch', (event) => {
  console.log('Service Worker fetching.');
  // Put in your logic to handle requests here
});
