#ifndef CPP_CONTENT_H
#define CPP_CONTENT_H

#include "records.h"
#include <string>

// Abstract base for posts and events, holding shared content identity.
class Content {
protected:
    int id;
    std::string title;
    std::string date;
    int authorId;
public:
    // Builds content from its common fields.
    Content(int contentId, const std::string &contentTitle, const std::string &contentDate, int contentAuthorId);
    // Enables safe deletion through a Content pointer.
    virtual ~Content();
    // Returns the content ID.
    int getId() const;
    // Returns the content title.
    std::string getTitle() const;
    // Returns the content date.
    std::string getDate() const;
    // Returns the author or organizer ID.
    int getAuthorId() const;
    // Validates and changes the title.
    bool setTitle(const std::string &value);
    // Validates and changes the date.
    bool setDate(const std::string &value);
    // Displays the derived record with an author name.
    virtual void display(const std::string &authorName) const = 0;
    // Returns the derived record type name.
    virtual std::string typeName() const = 0;
};

// OOP post model that converts to and from the C PostRec.
class Post : public Content {
    std::string body;
    int category;
public:
    // Builds a post from individual values.
    Post(int postId, const std::string &postTitle, const std::string &postBody, int postCategory, int postAuthorId, const std::string &postDate);
    // Builds a post from a C record.
    Post(const PostRec &record);
    // Converts this object to a C record.
    PostRec toRec() const;
    // Returns a readable category label.
    std::string categoryName() const;
    // Returns the post body.
    std::string getBody() const;
    // Displays a post card.
    void display(const std::string &authorName) const override;
    // Returns "Post".
    std::string typeName() const override;
};

// OOP event model that converts to and from the C EventRec.
class Event : public Content {
    std::string description;
    std::string venue;
public:
    // Builds an event from individual values.
    Event(int eventId, const std::string &eventTitle, const std::string &eventDescription, const std::string &eventVenue, const std::string &eventDate, int organizerId);
    // Builds an event from a C record.
    Event(const EventRec &record);
    // Converts this object to a C record.
    EventRec toRec() const;
    // Returns the event description.
    std::string getDescription() const;
    // Returns the event venue.
    std::string getVenue() const;
    // Displays an event card.
    void display(const std::string &authorName) const override;
    // Returns "Event".
    std::string typeName() const override;
};

#endif
