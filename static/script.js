async function updateDashboard() {
    try {
        const response = await fetch('/api/data');
        const data = await response.json();

        const stockList = document.getElementById('stock-list');
        stockList.innerHTML = '';
        data.stock.forEach(prod => {
            const isOut = prod.quantity === 0;
            stockList.innerHTML += `
                <div class="stock-item ${isOut ? 'danger' : ''}">
                    <div>
                        <strong style="font-size: 1.05rem;">${prod.name}</strong><br>
                        <span style="color: #64748b; font-size: 0.85rem;">ID المنتج: ${prod.id}</span>
                    </div>
                    <div class="tag">${isOut ? 'En rupture' : prod.quantity + ' Unités'}</div>
                </div>
            `;
        });

       
        const orderList = document.getElementById('order-list');
        orderList.innerHTML = '';

        if (data.orders.length === 0) {
            orderList.innerHTML = `<div style="text-align:center; padding: 2rem; color: #64748b; font-weight: 500;">🎉 File d'attente vide. Tous les ordres ont été traités!</div>`;
        } else {
            data.orders.forEach((cmd, index) => {
                const isFirst = index === 0; 
                orderList.innerHTML += `
                    <div class="card-order ${isFirst ? 'next-in-line' : ''}">
                        <div class="q-badge">${index + 1}</div>
                        <div>
                            <strong>Client: ${cmd.client}</strong>
                            <div style="font-size: 0.85rem; color: #64748b;">Produit: ${cmd.produit}</div>
                        </div>
                    </div>
                `;
            });
        }
    } catch (err) {
        console.error("Erreur lors de la synchronisation des données:", err);
    }
}

document.getElementById('order-form').addEventListener('submit', async (e) => {
    e.preventDefault(); 
    
    const client = document.getElementById('client-name').value;
    const produit = document.getElementById('product-name').value;

    const response = await fetch('/api/process', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ action: "add", client: client, produit: produit })
    });
    
    const result = await response.json();
    showNotification(result.message, result.status);
    
    document.getElementById('client-name').value = ''; 
    updateDashboard(); 
});

document.getElementById('btn-process').addEventListener('click', async () => {
    const response = await fetch('/api/process', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ action: "process" })
    });
    const result = await response.json();
    
    showNotification(result.message, result.status);
    updateDashboard(); 
});

function showNotification(msg, status) {
    const notif = document.getElementById('notification');
    notif.innerText = msg;
    notif.className = `show ${status}`;
    setTimeout(() => { notif.className = ''; }, 3500); 
}

window.onload = updateDashboard;