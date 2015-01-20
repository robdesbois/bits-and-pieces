/// 'Private Virtual' or 'Template Method' idiom
/// see Meyers' Effective C++

struct NetworkItemModel
{
    /// public function is non-virtual.
    void removeItem(int i)
    {
        auto svc = m_items[i];
        m_items.erase(i);
        onRemoveItem(svc);
        delete svc;
    }

private:
    /// private virtual function provides customisation point for derived classes
    /// empty body is a sensible default implementation
    virtual void onRemoveItem(NetworkItem& item)
    {}

    std::vector<NetworkItem> m_items;
};


struct ServiceModel: public NetworkItemModel
{
private:
    /// Derived type overrides the private virtual function
    virtual void onRemoveItem(NetworkItem& svc)
    {
        // derived type's custom code here
    }
};
