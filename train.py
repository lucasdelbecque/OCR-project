import torch
import torch.nn as nn
import torch.optim as optim
from torchvision import datasets, transforms
import os

NUM_FILTERS = 8


transform = transforms.Compose([
    transforms.RandomAffine(degrees=15, translate=(0.2, 0.2), scale=(0.8, 1.2)),
    transforms.ToTensor()
])

train_dataset = datasets.MNIST('./data', train=True, download=True, transform=transform)
train_loader = torch.utils.data.DataLoader(train_dataset, batch_size=64, shuffle=True)

class RobustCNN(nn.Module):
    def __init__(self):
        super(RobustCNN, self).__init__()
        self.conv1 = nn.Conv2d(1, NUM_FILTERS, kernel_size=3)
        self.fc1 = nn.Linear(13 * 13 * NUM_FILTERS, 10)

    def forward(self, x):
        x = torch.relu(self.conv1(x))
        x = torch.max_pool2d(x, 2)
        x = x.view(-1, 13 * 13 * NUM_FILTERS)
        x = self.fc1(x)
        return x

model = RobustCNN()
optimizer = optim.Adam(model.parameters(), lr=0.002)
criterion = nn.CrossEntropyLoss()


model.train()
for epoch in range(10):
    total_loss = 0
    for batch_idx, (data, target) in enumerate(train_loader):
        optimizer.zero_grad()
        output = model(data)
        loss = criterion(output, target)
        loss.backward()
        optimizer.step()
        total_loss += loss.item()
    print(f"Epoch {epoch+1}/10 - Moyenne Loss: {total_loss/len(train_loader):.4f}")

def save_matrix(filename, data):
    flattened = data.flatten()
    with open(filename, 'w') as f:
        for val in flattened:
            f.write(f"{val} ")
    print(f"Saved {filename}")

kernels = model.conv1.weight.data.numpy()
bias_conv = model.conv1.bias.data.numpy()
for i in range(NUM_FILTERS):
    save_matrix(f"conv_k{i}.txt", kernels[i, 0])
save_matrix("conv_bias.txt", bias_conv)

dense_w = model.fc1.weight.data.numpy()
dense_b = model.fc1.bias.data.numpy()
save_matrix("dense_weights.txt", dense_w)
save_matrix("dense_bias.txt", dense_b)
