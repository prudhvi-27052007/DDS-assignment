import os
import pickle

# ---------------- Contact Node ----------------
class ContactNode:
    def __init__(self, name, phone, email):
        self.name = name
        self.phone = phone
        self.email = email
        self.next = None  # self-referential structure


# ---------------- Contact Book ----------------
class ContactBook:
    def __init__(self, filename="contacts.dat"):
        self.head = None
        self.filename = filename
        self.load()

    # Insert in alphabetical order
    def add_contact(self, name, phone, email):
        new_node = ContactNode(name, phone, email)

        # Insert at beginning if empty or before head
        if self.head is None or name.lower() < self.head.name.lower():
            new_node.next = self.head
            self.head = new_node
        else:
            prev, curr = None, self.head
            while curr and name.lower() > curr.name.lower():
                prev, curr = curr, curr.next
            new_node.next = curr
            prev.next = new_node
        print(f"Contact '{name}' added.")
        self.save()

    # Search by name
    def search_contact(self, name):
        curr = self.head
        while curr:
            if curr.name.lower() == name.lower():
                print(f"Found: {curr.name} | {curr.phone} | {curr.email}")
                return curr
            curr = curr.next
        print("Contact not found.")
        return None

    # Update contact by name
    def update_contact(self, name, phone=None, email=None):
        node = self.search_contact(name)
        if node:
            if phone: node.phone = phone
            if email: node.email = email
            print(f"Contact '{name}' updated.")
            self.save()

    # Delete contact by name
    def delete_contact(self, name):
        curr, prev = self.head, None
        while curr:
            if curr.name.lower() == name.lower():
                if prev: prev.next = curr.next
                else: self.head = curr.next
                print(f"Contact '{name}' deleted.")
                self.save()
                return
            prev, curr = curr, curr.next
        print("Contact not found.")

    # Show all contacts
    def display_contacts(self):
        if not self.head:
            print("No contacts available.")
            return
        curr = self.head
        print("\n--- Contact List ---")
        while curr:
            print(f"{curr.name} | {curr.phone} | {curr.email}")
            curr = curr.next
        print("-------------------")

    # File save
    def save(self):
        with open(self.filename, "wb") as f:
            pickle.dump(self.head, f)

    # File load
    def load(self):
        if os.path.exists(self.filename):
            with open(self.filename, "rb") as f:
                try:
                    self.head = pickle.load(f)
                except EOFError:
                    self.head = None


# ---------------- Menu ----------------
def main():
    book = ContactBook()

    while True:
        print("\n1. Add Contact")
        print("2. Search Contact")
        print("3. Update Contact")
        print("4. Delete Contact")
        print("5. Show All Contacts")
        print("0. Exit")
        choice = input("Enter choice: ")

        if choice == "1":
            name = input("Name: ")
            phone = input("Phone: ")
            email = input("Email: ")
            book.add_contact(name, phone, email)
        elif choice == "2":
            book.search_contact(input("Enter name to search: "))
        elif choice == "3":
            name = input("Enter name to update: ")
            phone = input("New phone (or leave blank): ")
            email = input("New email (or leave blank): ")
            book.update_contact(name, phone if phone else None, email if email else None)
        elif choice == "4":
            book.delete_contact(input("Enter name to delete: "))
        elif choice == "5":
            book.display_contacts()
        elif choice == "0":
            print("Exiting... Goodbye!")
            break
        else:
            print("Invalid choice.")

if __name__ == "__main__":
    main()
